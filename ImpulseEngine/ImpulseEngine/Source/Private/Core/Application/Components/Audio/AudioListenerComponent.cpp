#include "gepch.h"
#include "Public/Core/Application/Components/Audio/AudioListenerComponent.h"
#include "Public/Core/Audio/AudioManager.h"
#include "Public/Core/Application/Entity.h"
#include "Public/Core/Application/Components/TransformComponent.h"

namespace GEngine {


	AudioListenerComponent::AudioListenerComponent(Entity* e) : NativeScript(e)
	{
		go_tag = "Audio Listener Component";
	}

	AudioListenerComponent::~AudioListenerComponent()
	{

	}

	void AudioListenerComponent::SetPitch(float f)
	{
		AudioManager::SetListenerPitch(f);
	}

	void AudioListenerComponent::SetVolume(float f)
	{
		AudioManager::SetListenerVolume(f);
	}

	const Vector3f AudioListenerComponent::GetPosition()
	{
		return AudioManager::GetListenerPosition();
	}

	const float AudioListenerComponent::GetPitch()
	{
		return AudioManager::GetListenerPitch();
	}

	const float AudioListenerComponent::GetVolume()
	{
		return AudioManager::GetListenerVolume();
	}

	void AudioListenerComponent::OnBegin()
	{
		AudioManager::SetListenerPosition(m_entity->GetTransform()->GetWorldPosition());
		m_entity->GetTransform()->AddTransformCallback(GetHash(), [this](Transform* transform, TransformData transData) {
			if (IsInitialized()) {
				AudioManager::SetListenerPosition(transform->GetWorldPosition());
			}
			});
	}

	void AudioListenerComponent::OnEnd()
	{
		m_entity->GetTransform()->RemoveTransformCallback(GetHash());
		Vector3f pos = Vector3f(0, 0, 0);
		AudioManager::SetListenerPosition(pos);
	}

}