#include "gepch.h"

#include "Public/Core/Audio/AudioContext.h"

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

}