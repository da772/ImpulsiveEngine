#include "gepch.h"

#include "Public/Core/Platform/Audio/OpenAL/OpenAL_source.h"
#include "Public/Core/Audio/AudioManager.h"

#define AL_LIBTYPE_STATIC
#include <AL/al.h>
#include <AL/alc.h>


namespace GEngine {

	OpenAL_source::OpenAL_source()
	{

	}

	OpenAL_source::~OpenAL_source()
	{

	}


	static bool isPlaying(uint32_t source, bool m_unloaded = false) {
		if (m_unloaded)
			return true;
		ALenum state;
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING;

	}

	void OpenAL_source::Play()
	{
		if (!IsPlaying()) {
			b_isPlaying = true;
			alSourcePlay(m_audioData.source);
		}
	}

	void OpenAL_source::Pause()
	{
		b_isPlaying = false;
		alSourceStop(m_audioData.source);	
	}

    void OpenAL_source::Unload() {
		if (IsPlaying()) {
			m_unloaded = true;
			alGetSourcei(m_audioData.source, AL_SAMPLE_OFFSET, &m_loadPos);
			alSourceStop(m_audioData.source);
		}
    }

    void OpenAL_source::Reload() {
		if (IsPlaying()) {
			if (m_unloaded) {
				alSourcei(m_audioData.source, AL_SAMPLE_OFFSET, m_loadPos);
				m_loadPos = 0;
				m_unloaded = false;
			}
			alSourcePlay(m_audioData.source);
		}
    }

	bool OpenAL_source::IsPlaying()
	{
		b_isPlaying = isPlaying(m_audioData.source, m_unloaded);
		return b_isPlaying;
	}

	void OpenAL_source::SetStatic(bool b)
	{
		b_isStatic = b;
		alSourcei(m_audioData.source, AL_SOURCE_RELATIVE, b);
	}

	void OpenAL_source::SetLoop(bool b)
	{
		b_loop = b;
		alSourcei(m_audioData.source, AL_LOOPING, b);
	}

	void OpenAL_source::Destroy()
	{
		AudioManager::RemoveSource(self.lock());
	}

	void OpenAL_source::SetVolume(float f)
	{
		alSourcef(m_audioData.source, AL_GAIN, f);
	}

	void OpenAL_source::SetPitch(float f)
	{
		alSourcef(m_audioData.source, AL_PITCH, f);
	}

	void OpenAL_source::Seek(float time)
	{
		int tot = 0;
		alGetBufferi(m_audioData.buffers[0], AL_SIZE, &tot);
		alSourcei(m_audioData.source, AL_BYTE_OFFSET, tot * time);
	}

	void OpenAL_source::SetPosition(const glm::vec3& pos)
	{
		if (b_isStatic) return;
		AudioSource::SetPosition(pos);
		alSource3f(m_audioData.source, AL_POSITION, pos.x, pos.y, pos.z);
	}

	void OpenAL_source::MaxDistance(const float f)
	{
		alSourcef(m_audioData.source, AL_ROLLOFF_FACTOR, 0.7f);
		alSourcef(m_audioData.source, AL_MAX_DISTANCE, f);
	}

}
