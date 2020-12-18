#include "Character/CharacterEntity.h"
#include "Character/CharacterController.hpp"
#include "Character/CharacterGraphics.hpp"
#include "Character/CharacterBody.hpp"

CharacterEntity::CharacterEntity(const Vector2f& position) : m_position(position)
{

}

void CharacterEntity::OnBegin()
	{

		SetEntityPosition({ m_position.x, m_position.y,0.f });

		m_characterComponent = CreateGameObject<CharacterController>();
		m_spriteComponent = CreateGameObject<CharacterGraphics>();
		m_audioComponent = CreateGameObject<AudioListenerComponent>();


		

        AddComponent(m_spriteComponent);
		AddComponent(m_characterComponent);
		AddComponent(m_audioComponent);
	}

	void CharacterEntity::OnEnd()
	{
		m_characterComponent = nullptr;
		m_spriteComponent = nullptr;
		m_audioComponent = nullptr;
	}

	void CharacterEntity::OnUpdate(Timestep timestep)
	{

	}
