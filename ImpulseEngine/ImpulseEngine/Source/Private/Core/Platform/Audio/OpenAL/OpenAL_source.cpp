#include "gepch.h"

#include "Public/Core/Platform/Audio/OpenAL/OpenAL_source.h"
#include "Public/Core/Audio/AudioManager.h"
#define AL_LIBTYPE_STATIC
#include <Al/al.h>
#include <AL/alc.h>


namespace GEngine {

	OpenAL_source::OpenAL_source(AudioStreamingData* data) : AudioSource(data)
	{

	}

	OpenAL_source::~OpenAL_source()
	{
		delete m_audioData;
	}

	void OpenAL_source::Play()
	{
		b_isPlaying = true;
		alSourcePlay(m_audioData->source);
	}

	void OpenAL_source::Pause()
	{
		b_isPlaying = false;
		alSourceStop(m_audioData->source);
	}

	void OpenAL_source::SetLoop(bool b)
	{
		alSourcei(m_audioData->source, AL_LOOPING, b);
	}

	void OpenAL_source::Destroy()
	{
		AudioManager::RemoveSource(self.lock());
	}

	void OpenAL_source::SetVolume(float f)
	{
		alSourcef(m_audioData->source, AL_GAIN, f);
	}

	void OpenAL_source::SetPitch(float f)
	{
		alSourcef(m_audioData->source, AL_PITCH, f);
	}

	void OpenAL_source::Seek(float time)
	{
		m_audioData->sizeConsumed = 0;
	}

}