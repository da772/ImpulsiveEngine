#include "gepch.h"

#include "Public/Core/Audio/AudioManager.h"

#include "Public/Core/Audio/AudioContext.h"

namespace GEngine {

	Ref<AudioContext> AudioManager::s_audioManager;

	void AudioManager::Initalize()
	{
		s_audioManager = AudioContext::Create();
	}

	void AudioManager::Shutdown()
	{
		s_audioManager = nullptr;
	}

	void AudioManager::SetListenerPosition(const glm::vec3& pos)
	{
		s_audioManager->SetListenerPosition(pos);
	}

	Ref<AudioSource> AudioManager::PlayOgg(std::string filename, bool fromPak /*= true*/, bool relative /*= true*/)
	{
		return s_audioManager->LoadSource(filename.c_str(), fromPak, relative);
	}

	void AudioManager::Update()
	{
		s_audioManager->Update();
	}

	void AudioManager::RemoveSource(Ref<AudioSource> s)
	{
		s_audioManager->Destroy(s);
	}

}