#include "gepch.h"
#include "Public/Platform/Audio/OpenAL/OpenAL_Context.h"
#include "Public/Platform/Audio/OpenAL/OpenAL_source.h"
#include "Public/Core/Audio/AudioSource.h"
#include "Public/Core/Util/ThreadPool.h"
#if defined(GE_AUDIO_OPENAL)
#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/efx.h>
#include <vorbis/vorbisfile.h>
#endif
#include "Public/Core/FileSystem/FileSystem.h"



namespace GEngine {

	
#if defined(GE_AUDIO_OPENAL)
	void check_al_errors(const std::string& filename, const std::uint_fast32_t line);

	template<typename alFunction, typename... Params>
	static auto alCallImpl(const char* filename, const std::uint_fast32_t line, alFunction function, Params... params)
		->typename std::enable_if<std::is_same<void, decltype(function(params...))>::value, decltype(function(params...))>::type
	{
		function(std::forward<Params>(params)...);
		check_al_errors(filename, line);
	}

	template<typename alFunction, typename... Params>
	static auto alCallImpl(const char* filename, const std::uint_fast32_t line, alFunction function, Params... params)
		->typename std::enable_if<!std::is_same<void, decltype(function(params...))>::value, decltype(function(params...))>::type
	{
		auto ret = function(std::forward<Params>(params)...);
		check_al_errors(filename, line);
		return ret;
	}
	#endif
	
#if defined(GE_AUDIO_OPENAL)
#define alCall(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)
#define alcCall(function, device, ...) alcCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)

	static std::size_t read_ogg_callback(void* destination, std::size_t size1, std::size_t size2, void* fileHandle);
	static int32_t seek_ogg_callback(void* fileHandle, ogg_int64_t to, std::int32_t type);
	static long int tell_ogg_callback(void* fileHandle);
	static int close_ogg_callback(void* fileHandle);
#endif
	OpenAL_Context::OpenAL_Context()
	{
		#if defined(GE_AUDIO_OPENAL)
		device = alcOpenDevice(NULL);
		ALCint attribs[] = {
		ALC_REFRESH, 60, ALC_SYNC, ALC_FALSE, ALC_MAX_AUXILIARY_SENDS, 2, 0
		};

		ctx = alcCreateContext((ALCdevice*)device, attribs);

	
		if (ctx == NULL || alcMakeContextCurrent((ALCcontext*)ctx) == ALC_FALSE)
		{
			if (ctx != NULL)
				alcDestroyContext((ALCcontext*)ctx);
			alcCloseDevice((ALCdevice*)device);
			GE_CORE_ERROR("COULD NOT SET ALC DEVICE");
		}
		alCall(alDistanceModel, AL_INVERSE_DISTANCE_CLAMPED);
		int major, minor;
		alcGetIntegerv((ALCdevice*)device, ALC_MAJOR_VERSION, sizeof(int), &major);
		alcGetIntegerv((ALCdevice*)device, ALC_MINOR_VERSION, sizeof(int), &minor);
		GE_CORE_INFO("Initalized: {0} - v{1}.{2} ", alcGetString((ALCdevice*)device, ALC_DEVICE_SPECIFIER), major, minor);

		alCall(alGetListener3f, AL_POSITION, &m_listenerPos.x, &m_listenerPos.y, &m_listenerPos.z);
		//alCall(alGetListenerf, AL_PITCH, &m_listenerPitch);
		alCall(alGetListenerf, AL_GAIN, &m_listenerVolume);
#endif
	}

	OpenAL_Context::~OpenAL_Context()
	{
		for (Ref<AudioSource> s : m_sources) {
			Destroy(s);
		}
#if defined(GE_AUDIO_OPENAL)
		alcDestroyContext((ALCcontext*)ctx);
		alcCloseDevice((ALCdevice*)device);
	#endif
	}



	void OpenAL_Context::UpdateStream(Ref<AudioSource> audioSource) {
		#if defined(GE_AUDIO_OPENAL)
		AudioStreamingData& audioData = audioSource->GetData();
		OggVorbis_File* oggFile = &dynamic_pointer_cast<OpenAL_source>(audioSource)->oggFile;

		ALint buffersProcessed = 0;
		alCall(alGetSourcei, audioData.source, AL_BUFFERS_PROCESSED, &buffersProcessed);
		if (buffersProcessed <= 0)
		{
			return;
		}
		if (buffersProcessed > audioData.bufferNum)
			buffersProcessed = audioData.bufferNum;
		while (buffersProcessed--)
		{
			ALuint buffer;
			if (audioData.queued) {
				alCall(alSourceUnqueueBuffers, audioData.source, 1, &buffer);
				audioData.queued = false;
			}

			char* data = new char[AUDIO_BUFFER_SIZE];
			std::memset(data, 0, AUDIO_BUFFER_SIZE);

			ALsizei dataSizeToBuffer = 0;
			std::int32_t sizeRead = 0;

			while (sizeRead < AUDIO_BUFFER_SIZE)
			{
				std::int32_t result = ov_read(oggFile, &data[sizeRead], AUDIO_BUFFER_SIZE - sizeRead, 0, 2, 1, &audioData.fileSection);
				if (result == OV_HOLE)
				{
					GE_CORE_ERROR("ERROR: OV_HOLE found in update of buffer ");
					break;
				}
				else if (result == OV_EBADLINK)
				{
					GE_CORE_ERROR("ERROR: OV_EBADLINK found in update of buffer ");
					break;
				}
				else if (result == OV_EINVAL)
				{
					GE_CORE_ERROR("ERROR: OV_EINVAL found in update of buffer ");
					break;
				}
				else if (result == 0)
				{
					if (!audioSource->IsLooping()) { break; }
					std::int32_t seekResult = ov_raw_seek(oggFile, 0);
					if (seekResult == OV_ENOSEEK)
						GE_CORE_ERROR("ERROR: OV_ENOSEEK found when trying to loop");
					else if (seekResult == OV_EINVAL)
						GE_CORE_ERROR("ERROR: OV_EINVAL found when trying to loop");
					else if (seekResult == OV_EREAD)
						GE_CORE_ERROR("ERROR: OV_EREAD found when trying to loop");
					else if (seekResult == OV_EFAULT)
						GE_CORE_ERROR("ERROR: OV_EFAULT found when trying to loop");
					else if (seekResult == OV_EOF)
						GE_CORE_ERROR("ERROR: OV_EOF found when trying to loop");
					else if (seekResult == OV_EBADLINK)
						GE_CORE_ERROR("ERROR: OV_EBADLINK found when trying to loop");

					if (seekResult != 0)
					{
						GE_CORE_ERROR("ERROR: Unknown error in ov_raw_seek");
						delete[] data;
						return;
					}
				}
				sizeRead += result;
			}
			dataSizeToBuffer = sizeRead;

			if (dataSizeToBuffer > 0)
			{
				
			}

			
			GE_CORE_ASSERT(!audioData.queued, "Buffer Already queued");
			alCall(alBufferData, buffer, audioData.format, data, dataSizeToBuffer, audioData.sampleRate);
			alCall(alSourceQueueBuffers, audioData.source, 1, &buffer);
			audioData.queued = true;

			

			ALint state;
			alCall(alGetSourcei, audioData.source, AL_SOURCE_STATE, &state);
			if (state == AL_PLAYING)
			{
				if (dataSizeToBuffer < AUDIO_BUFFER_SIZE)
				{
				}
				
				//alCall(alSourcePlay, audioData.source);
			}
			else {
				//GE_CORE_DEBUG("AUDIO STOPPED - {0}", audioData.fileName);
				audioSource->Pause();
				ResetBuffers(audioSource, buffer);
				if (audioSource->IsLooping()) audioSource->Play();
			}

			delete[] data;
		}
		#endif
	}
	
	void OpenAL_Context::Update()
	{
		/*
		for (Ref<AudioSource> s : m_sources) {
			if (s->IsPlaying()) {
				//UpdateStream(s);
			}
		}
		*/
	}


	void OpenAL_Context::Destroy(Ref<AudioSource> s)
	{
		if (m_sources.find(s) == m_sources.end())
			return;
		m_sources.erase(s);
		#if defined(GE_AUDIO_OPENAL)
        alSourceStop(s->GetData().source);
		alSourcei(s->GetData().source, AL_BUFFER, AL_NONE);
		alDeleteSources(1, &s->GetData().source);
		alDeleteBuffers(AUDIO_BUFFERS_NUM, &s->GetData().buffers[0]);
		ov_clear(&dynamic_pointer_cast<OpenAL_source>(s)->oggFile);
		#endif
	}

	void OpenAL_Context::SetListenerPosition(const Vector3f& pos)
	{
		m_listenerPos = pos;
		#if defined(GE_AUDIO_OPENAL)
		alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
		#endif
	}

	const Vector3f& OpenAL_Context::GetListenerPosition()
	{
		return m_listenerPos;
	}

	void OpenAL_Context::SetListenerPitch(float f)
	{
		m_listenerPitch = f;
		#if defined(GE_AUDIO_OPENAL)
		alListenerf(AL_PITCH, f);
		#endif
	}

	void OpenAL_Context::SetListenerVolume(float f)
	{
		m_listenerVolume = f;
		#if defined(GE_AUDIO_OPENAL)
		alListenerf(AL_GAIN, f);
		#endif
	}

	const float OpenAL_Context::GetListenerPitch()
	{
		return m_listenerPitch;
	}

	const float OpenAL_Context::GetListenerVolume()
	{
		return m_listenerVolume;
	}

	void OpenAL_Context::ResetBuffers(Ref<AudioSource> audioSource, uint32_t buffer)
	{
		#if defined(GE_AUDIO_OPENAL)
		OggVorbis_File& oggFile = dynamic_pointer_cast<OpenAL_source>(audioSource)->oggFile;
		AudioStreamingData* audioData = &audioSource->GetData();

		ALint buffersProcessed = 0;

		alCall(alGetSourcei, audioData->source, AL_BUFFERS_PROCESSED, &buffersProcessed);

		char* data = new char[AUDIO_BUFFER_SIZE];
		memset(data, 0, AUDIO_BUFFER_SIZE);
		ov_raw_seek(&oggFile, 0);
		for (std::uint8_t i = 0; i < audioData->bufferNum; i++)
		{
			std::int32_t dataSoFar = 0;
			while (dataSoFar < AUDIO_BUFFER_SIZE)
			{
				std::int32_t result = ov_read(&oggFile, &data[dataSoFar], AUDIO_BUFFER_SIZE - dataSoFar, 0, 2, 1, &audioData->fileSection);
				if (result == OV_HOLE)
				{
					GE_CORE_ERROR("ERROR: OV_HOLE found in initial read of buffer {0}", i);
					break;
				}
				else if (result == OV_EBADLINK)
				{
					GE_CORE_ERROR("ERROR: OV_EBADLINK found in initial read of buffer {0}", i);
					break;
				}
				else if (result == OV_EINVAL)
				{
					GE_CORE_ERROR("ERROR: OV_EINVAL found in inital read of buffer {0}", i);
					break;
				}
				else if (result == 0)
				{
					//GE_CORE_ERROR("ERROR: EOF found in initial read of buffer {0}", i);
					break;
				}

				dataSoFar += result;
			}

			if (audioData->queued) {
				alCall(alSourceUnqueueBuffers, audioData->source, 1, &buffer);
				audioData->queued = false;
			}

			if (dataSoFar > 0) {
				alCall(alBufferData, buffer, audioData->format, data, dataSoFar, audioData->sampleRate);
			}
		}
		GE_CORE_ASSERT(!audioData->queued, "Buffer Already queued");
		alCall(alSourceQueueBuffers, audioData->source, audioData->bufferNum, &audioData->buffers[0]);
		audioData->queued = true;
		delete[] data;
		#endif
	}

	Ref<AudioSource> OpenAL_Context::LoadSource(const char* fileName, bool fromPak /*= true*/, bool relative /*= true*/)
	{
		
		Ref<OpenAL_source> s = make_shared<OpenAL_source>();
		#if defined(GE_AUDIO_OPENAL)
		AudioStreamingData* audioData = &s->GetData();
		audioData->fileName = fileName;
		audioData->fromPak = fromPak;
		audioData->relative = relative;

		audioData->file = FileSystem::FileDataFromPath(fileName, !audioData->fromPak, audioData->relative);
		audioData->sizeConsumed = 0;
		audioData->size = audioData->file->GetDataSize();

		ov_callbacks oggCallbacks;
		oggCallbacks.read_func = read_ogg_callback;
		oggCallbacks.close_func = close_ogg_callback;
		oggCallbacks.seek_func = seek_ogg_callback;
		oggCallbacks.tell_func = tell_ogg_callback;

		OggVorbis_File& oggFile = s->oggFile;

		if (ov_open_callbacks(reinterpret_cast<void*>(audioData), &oggFile, nullptr, -1, oggCallbacks) < 0) {
			GE_CORE_ERROR("COULD NOT OPEN OV_OPEN_CALLBACKS");
			GE_CORE_ASSERT(false, "");
			return nullptr;
		}

		audioData->nativeFile = (void*)&oggFile;

		vorbis_info* vorbisInfo = ov_info(&oggFile, -1);
		audioData->channels = vorbisInfo->channels;
		audioData->bitsPerSample = 16;
		audioData->sampleRate = vorbisInfo->rate;
		audioData->duration = ov_time_total(&oggFile, -1);
		if (audioData->channels == 1 && audioData->bitsPerSample == 8)
			audioData->format = AL_FORMAT_MONO8;
		else if (audioData->channels == 1 && audioData->bitsPerSample == 16)
			audioData->format = AL_FORMAT_MONO16;
		else if (audioData->channels == 2 && audioData->bitsPerSample == 8)
			audioData->format = AL_FORMAT_STEREO8;
		else if (audioData->channels == 2 && audioData->bitsPerSample == 16)
			audioData->format = AL_FORMAT_STEREO16;


		size_t data_len = ov_pcm_total(&oggFile, -1) * audioData->channels * 2;
		char* pcmout = (char*)malloc(data_len);

		for (size_t size = 0, offset = 0, sel = 0; (size = ov_read(&oggFile, pcmout + offset, 4096, 0, 2, 1, (int*)&sel)) != 0; offset += size) {
			if (size < 0) {
				GE_CORE_ERROR("OpenAL: InCorrect OGG FILE");
			}
		}



		alCall(alGenSources, (ALsizei)1, (ALuint*)&audioData->source);
		
		alCall(alSourcef, audioData->source, AL_PITCH, 1.f);
		alCall(alSourcef, audioData->source, AL_GAIN, .5f);
		alCall(alSource3f, audioData->source, AL_POSITION, 0, 0, 0);
		alCall(alSource3f, audioData->source, AL_VELOCITY, 0, 0, 0);
		alCall(alSourcei, audioData->source, AL_LOOPING, AL_FALSE);
		

		

		alCall(alGenBuffers, AUDIO_BUFFERS_NUM, &audioData->buffers[0]);
		int bufferNum = 1;

		alCall(alBufferData, audioData->buffers[0], audioData->format, pcmout, data_len, audioData->sampleRate);
		alCall(alSourcei, audioData->source, AL_BUFFER, audioData->buffers[0]);
		//alCall(alSourceQueueBuffers, audioData->source, audioData->bufferNum, &audioData->buffers[0]);
		alCall(alSourcePlay, s->GetData().source);
		free((void*)pcmout);


		/*
		char* data = new char[AUDIO_BUFFER_SIZE];

		for (std::uint8_t i = 0; i < AUDIO_BUFFERS_NUM; ++i)
		{
			std::int32_t dataSoFar = 0;
			while (dataSoFar < AUDIO_BUFFER_SIZE)
			{
				std::int32_t result = ov_read(&oggFile, &data[dataSoFar], AUDIO_BUFFER_SIZE - dataSoFar, 0, 2, 1, &audioData->fileSection);
				if (result == OV_HOLE)
				{
					GE_CORE_ERROR("ERROR: OV_HOLE found in initial read of buffer {0}", i);
					break;
				}
				else if (result == OV_EBADLINK)
				{
					GE_CORE_ERROR("ERROR: OV_EBADLINK found in initial read of buffer {0}", i);
					break;
				}
				else if (result == OV_EINVAL)
				{
					GE_CORE_ERROR("ERROR: OV_EINVAL found in inital read of buffer {0}", i);
					break;
				}
				else if (result == 0)
				{
					//GE_CORE_ERROR("ERROR: EOF found in initial read of buffer {0}", i);
					if (bufferNum == AUDIO_BUFFERS_NUM)
						bufferNum = i + 1;
					break;
				}

				dataSoFar += result;
			}

			if (audioData->channels == 1 && audioData->bitsPerSample == 8)
				audioData->format = AL_FORMAT_MONO8;
			else if (audioData->channels == 1 && audioData->bitsPerSample == 16)
				audioData->format = AL_FORMAT_MONO16;
			else if (audioData->channels == 2 && audioData->bitsPerSample == 8)
				audioData->format = AL_FORMAT_STEREO8;
			else if (audioData->channels == 2 && audioData->bitsPerSample == 16)
				audioData->format = AL_FORMAT_STEREO16;
			else
			{
				GE_CORE_ERROR("OpenAL: Unrecongnized ogg fomrat: {0} channels, {1} bps", audioData->channels, audioData->bitsPerSample);
				delete[] data;
				return nullptr;
			}

			alCall(alBufferData, audioData->buffers[i], audioData->format, data, dataSoFar, audioData->sampleRate);
		}

		GE_CORE_ASSERT(!audioData->queued, "Buffer Already queued");
		audioData->bufferNum = bufferNum;
		alCall(alSourceQueueBuffers, audioData->source, audioData->bufferNum, &audioData->buffers[0]);
		audioData->queued = true;
		
		delete[] data;
		*/
		s->SetSelf(s);
		s->oggFile = std::move(oggFile);
		m_sources.insert(s);
		#endif
		return s;
	}

	int close_ogg_callback(void* fileHandle) {
		return 0;
	}
#if defined(GE_AUDIO_OPENAL)
	std::size_t read_ogg_callback(void* destination, std::size_t size1, std::size_t size2, void* fileHandle)
	{
		
		AudioStreamingData* audioData = reinterpret_cast<AudioStreamingData*>(fileHandle);
		uint32_t length = size1 * size2;

		if (audioData->sizeConsumed + length > audioData->size)
			length = audioData->size - audioData->sizeConsumed;

		if (audioData->sizeConsumed < audioData->size) {
			memcpy(destination, audioData->file->GetData() + audioData->sizeConsumed, length);
			audioData->sizeConsumed += length;
		}

		return length;
	}


	std::int32_t seek_ogg_callback(void* fileHandle, ogg_int64_t to, std::int32_t type)
	{
		AudioStreamingData* audioData = reinterpret_cast<AudioStreamingData*>(fileHandle);

		if (type == SEEK_CUR)
		{
			audioData->sizeConsumed += to;
		}
		else if (type == SEEK_END)
		{
			audioData->sizeConsumed = audioData->size - to;
		}
		else if (type == SEEK_SET)
		{
			audioData->sizeConsumed = to;
		}
		else
			return -1; // what are you trying to do vorbis?

		if (audioData->sizeConsumed < 0)
		{
			audioData->sizeConsumed = 0;
			return -1;
		}
		if (audioData->sizeConsumed > audioData->size)
		{
			audioData->sizeConsumed = audioData->size;
			return -1;
		}

		return 0;
	}

	long int tell_ogg_callback(void* fileHandle)
	{
		AudioStreamingData* audioData = reinterpret_cast<AudioStreamingData*>(fileHandle);
		return audioData->sizeConsumed;
	}


	void check_al_errors(const std::string& filename, const std::uint_fast32_t line)
	{
		ALCenum error = alGetError();
		if (error != AL_NO_ERROR)
		{
			switch (error)
			{
			case AL_INVALID_NAME:
				GE_CORE_DEBUG("{0} : {1} - AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function", filename, line);
				break;
			case AL_INVALID_ENUM:
				GE_CORE_DEBUG("{0} : {1} - AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function", filename, line);
				break;
			case AL_INVALID_VALUE:
				GE_CORE_DEBUG("{0} : {1} - AL_INVALID_VALUE: an invalid value was passed to an OpenAL function", filename, line);
				break;
			case AL_INVALID_OPERATION:
				GE_CORE_DEBUG("{0} : {1} - AL_INVALID_OPERATION: the requested operation is not valid", filename, line);
				break;
			case AL_OUT_OF_MEMORY:
				GE_CORE_DEBUG("{0} : {1} - AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory", filename, line);
				break;
			default:
				GE_CORE_DEBUG("{0} : {1} - UNKNOWN AL ERROR", filename, line);
			}
		}
	}
#endif
}
