#include "gepch.h"

#include "Public/Core/Audio/AudioManager.h"
#include "Public/Core/Audio/AudioContext.h"

namespace GEngine {

	Ref<AudioContext> AudioManager::s_audioContext;

	void AudioManager::Initalize()
	{
		s_audioContext = AudioContext::Create();
	}

	void AudioManager::Shutdown()
	{
		s_audioContext = nullptr;
	}

	void AudioManager::SetListenerPosition(const glm::vec3& pos)
	{
		s_audioContext->SetListenerPosition(pos);
	}

	const glm::vec3 AudioManager::GetListenerPosition()
	{
		return s_audioContext->GetListenerPosition();
	}

	Ref<AudioSource> AudioManager::Load_OGG(std::string filename, bool fromPak /*= true*/, bool relative /*= true*/)
	{
		return s_audioContext->LoadSource(filename.c_str(), fromPak, relative);
	}

	void AudioManager::SetListenerVolume(float f)
	{
		s_audioContext->SetListenerVolume(f);
	}

	void AudioManager::SetListenerPitch(float f)
	{
		s_audioContext->SetListenerPitch(f);
	}

	const float AudioManager::GetListenerPitch()
	{
		return s_audioContext->GetListenerPitch();
	}

	const float AudioManager::GetListenerVolume()
	{
		return s_audioContext->GetListenerVolume();
	}

	void AudioManager::Update()
	{
		s_audioContext->Update();
	}

	void AudioManager::Pause() {
		s_audioContext->Pause();
	}

	void AudioManager::Resume() {
		s_audioContext->Resume();
	}

	void AudioManager::RemoveSource(Ref<AudioSource> s)
	{
		s_audioContext->Destroy(s);
	}

	void AudioManager::ResetBuffers(Ref<AudioSource> s)
	{
		s_audioContext->ResetBuffers(s);
	}

}