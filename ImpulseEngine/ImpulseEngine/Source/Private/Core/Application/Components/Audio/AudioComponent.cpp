
#include "gepch.h"
#include "Public/Core/Application/Components/Audio/AudioComponent.h"
#include "Public/Core/Audio/AudioManager.h"
#include "Public/Core/Audio/AudioSource.h"
#include "Public/Core/Application/Entity.h"
#include "Public/Core/Application/Components/TransformComponent.h"

namespace GEngine {

	AudioComponent::AudioComponent(Entity* e, const std::string& fileName, bool playing, bool looping, bool bStatic, float volume, float pitch, bool fromPak /*= true*/, bool relative /*= true*/) : Component(e)
	{
		go_tag = "Audio Component";
		m_fileName = fileName;
		m_fromPak = fromPak;
		m_relative = relative;
		m_volume = volume;
		m_pitch = pitch;
		m_playing = playing;
		m_looping = looping;
		m_static = bStatic;
		
	}

	AudioComponent::~AudioComponent()
	{
		if (m_audioSource != nullptr) {
			m_audioSource->Destroy();
			m_audioSource = nullptr;
		}
	}

	bool AudioComponent::IsPlaying()
	{
		return m_audioSource->IsPlaying();
	}

	bool AudioComponent::IsLooping()
	{
		return m_audioSource->IsLooping();
	}

	void AudioComponent::SetPlaying(bool b)
	{
		if (b)
			m_audioSource->Play();
		else
			m_audioSource->Pause();
	}

	void AudioComponent::SetLooping(bool b)
	{
		m_audioSource->SetLoop(b);
	}

	void AudioComponent::Restart()
	{
		bool b = IsPlaying();
		SetPlaying(false);
		m_audioSource->Seek(0);
		if (b)
			SetPlaying(true);
	}

	void AudioComponent::SetPitch(float f)
	{
		m_pitch = f;
		if (m_audioSource) {
			m_audioSource->SetPitch(m_pitch);
		}
	}

	void AudioComponent::SetVolume(float f)
	{
		m_volume = f;
		if (m_audioSource) {
			m_audioSource->SetVolume(m_volume);
		}

	}

	void AudioComponent::SetStatic(bool b)
	{
		m_audioSource->SetStatic(b);
	}

	bool AudioComponent::GetStatic()
	{
		return m_audioSource->GetStatic();
	}

	void AudioComponent::OnBegin()
	{
		GE_CORE_ASSERT(m_audioSource == nullptr, "AUDIO SOURCE ALREADY INSTANTIATED");
		m_audioSource = AudioManager::Load_OGG(m_fileName, m_fromPak, m_relative);
		m_audioSource->SetPosition(m_entity->GetPosition());
		m_audioSource->SetPitch(m_pitch);
		m_audioSource->SetVolume(m_volume);
		m_audioSource->SetStatic(m_static);
		m_audioSource->MaxDistance(50.f);
		SetPlaying(m_playing);
		SetLooping(m_looping);
		m_entity->AddTransformCallback(this, [this](Transform* transform, TransformData transData) {
			if (IsInitialized()) {
				m_audioSource->SetPosition(transform->GetPosition());
			}
			});
	}

	void AudioComponent::OnEnd()
	{
		m_entity->RemoveTransformCallback(this);
		m_audioSource->Destroy();
		m_audioSource = nullptr;
	}

}
