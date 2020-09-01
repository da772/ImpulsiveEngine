#include "gepch.h"

#include "Public/Core/Audio/AudioContext.h"
#include "Public/Core/Audio/AudioSource.h"

#ifdef GE_AUDIO_OPENAL

#include "Public/Core/Platform/Audio/OpenAL/OpenAL_Context.h"

#endif

namespace GEngine {

	GEngine::Ref<GEngine::AudioContext> AudioContext::Create()
	{
#ifdef GE_AUDIO_OPENAL
		return make_shared<OpenAL_Context>();
#endif

		return nullptr;
	}

	void AudioContext::Pause()
	{
		for (Ref<AudioSource> s : m_sources) {
                s->Unload();
		}
	}

	void AudioContext::Resume()
	{
		for (Ref<AudioSource> s : m_sources) {
            s->Reload();
		}
	}

}
