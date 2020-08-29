#include "gepch.h"
#include "Public/Core/Application/Components/Audio/AudioListenerComponent.h"
#include "Public/Core/Audio/AudioManager.h"
#include "Public/Core/Application/Entity.h"
#include "Public/Core/Application/Components/TransformComponent.h"

namespace GEngine {


	AudioListenerComponent::AudioListenerComponent()
	{

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

	const glm::vec3 AudioListenerComponent::GetPosition()
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

	void AudioListenerComponent::OnAttached(Ref<Entity> entity)
	{
		entity->AddTransformCallback(std::static_pointer_cast<Component>(self.lock()), [this](Ref<Transform> transform, TransformData transData) {
			if (IsInitialized()) {
				AudioManager::SetListenerPosition(transform->GetPosition());
			}
			});
	}

	void AudioListenerComponent::DeAttached(Ref<Entity> entity)
	{
		entity->RemoveTransformCallback(std::static_pointer_cast<Component>(self.lock()));
	}

	void AudioListenerComponent::OnBegin()
	{
		AudioManager::SetListenerPosition(entity.lock()->GetEntityPosition());
	}

	void AudioListenerComponent::OnEnd()
	{
		glm::vec3 pos = glm::vec3(0, 0, 0);
		AudioManager::SetListenerPosition(pos);
	}

}