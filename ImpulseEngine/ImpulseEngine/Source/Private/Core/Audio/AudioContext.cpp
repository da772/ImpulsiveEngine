#include "gepch.h"

#include "Public/Core/Audio/AudioContext.h"
#include "Public/Core/Audio/AudioSource.h"

#ifdef GE_AUDIO_OPENAL
#include "Public/Platform/Audio/OpenAL/OpenAL_Context.h"
#endif

#ifdef GE_AUDIO_NONE
#include "Public/Platform/Audio/Server/Server_AudioContext.h"
#endif

namespace GEngine {

	GEngine::Ref<GEngine::AudioContext> AudioContext::Create()
	{
#ifdef GE_AUDIO_OPENAL
		return std::make_shared<OpenAL_Context>();
#endif
#ifdef GE_AUDIO_NONE
		return std::make_shared<Server_AudioContext>();
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
