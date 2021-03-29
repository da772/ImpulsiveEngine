#include "gepch.h"

#include "Public/Platform/Audio/OpenAL/OpenAL_source.h"
#include "Public/Core/Audio/AudioManager.h"
#if defined (GE_AUDIO_OPENAL)
#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>
#endif

namespace GEngine {

	OpenAL_source::OpenAL_source()
	{

	}

	OpenAL_source::~OpenAL_source()
	{

	}


	static bool isPlaying(uint32_t source, bool m_unloaded = false) {
		#if defined (GE_AUDIO_OPENAL)
		if (m_unloaded)
			return true;
		ALenum state;
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING;
		#else
		return false;
		#endif
	}

	void OpenAL_source::Play()
	{
		#if defined (GE_AUDIO_OPENAL)
		if (!IsPlaying()) {
			b_isPlaying = true;
			alSourcePlay(m_audioData.source);
		}
		#endif
	}

	void OpenAL_source::Pause()
	{
		#if defined (GE_AUDIO_OPENAL)
		b_isPlaying = false;
		alSourceStop(m_audioData.source);	
		#endif
	}

    void OpenAL_source::Unload() {
		#if defined (GE_AUDIO_OPENAL)
		if (IsPlaying()) {
			m_unloaded = true;
			alGetSourcei(m_audioData.source, AL_SAMPLE_OFFSET, &m_loadPos);
			alSourceStop(m_audioData.source);
		}
		#endif
    }

    void OpenAL_source::Reload() {
		#if defined (GE_AUDIO_OPENAL)
		if (IsPlaying()) {
			if (m_unloaded) {
				alSourcei(m_audioData.source, AL_SAMPLE_OFFSET, m_loadPos);
				m_loadPos = 0;
				m_unloaded = false;
			}
			alSourcePlay(m_audioData.source);
		}
		#endif
    }

	bool OpenAL_source::IsPlaying()
	{
		b_isPlaying = isPlaying(m_audioData.source, m_unloaded);
		return b_isPlaying;
	}

	void OpenAL_source::SetStatic(bool b)
	{
		#if defined (GE_AUDIO_OPENAL)
		b_isStatic = b;
		alSourcei(m_audioData.source, AL_SOURCE_RELATIVE, b);
		#endif
	}

	void OpenAL_source::SetLoop(bool b)
	{
		#if defined (GE_AUDIO_OPENAL)
		b_loop = b;
		alSourcei(m_audioData.source, AL_LOOPING, b);
		#endif
	}

	void OpenAL_source::Destroy()
	{
		AudioManager::RemoveSource(self.lock());
	}

	void OpenAL_source::SetVolume(float f)
	{
		#if defined (GE_AUDIO_OPENAL)
		alSourcef(m_audioData.source, AL_GAIN, f);
		#endif
	}

	void OpenAL_source::SetPitch(float f)
	{
		#if defined (GE_AUDIO_OPENAL)
		alSourcef(m_audioData.source, AL_PITCH, f);
		#endif
	}

	void OpenAL_source::Seek(float time)
	{
		#if defined (GE_AUDIO_OPENAL)
		int tot = 0;
		alGetBufferi(m_audioData.buffers[0], AL_SIZE, &tot);
		alSourcei(m_audioData.source, AL_BYTE_OFFSET, tot * time);
		#endif
	}

	void OpenAL_source::SetPosition(const Vector3f& pos)
	{
		#if defined (GE_AUDIO_OPENAL)
		if (b_isStatic) return;
		AudioSource::SetPosition(pos);
		alSource3f(m_audioData.source, AL_POSITION, pos.x, pos.y, pos.z);
		#endif
	}

	void OpenAL_source::MaxDistance(const float f)
	{
		#if defined (GE_AUDIO_OPENAL)
		alSourcef(m_audioData.source, AL_ROLLOFF_FACTOR, 0.7f);
		alSourcef(m_audioData.source, AL_MAX_DISTANCE, f);
		#endif
	}

}
