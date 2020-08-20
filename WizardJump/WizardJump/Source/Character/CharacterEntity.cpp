#include "Character/CharacterEntity.h"
#include "Character/CharacterController.hpp"
#include "Character/CharacterGraphics.hpp"
#include "Character/CharacterBody.hpp"

	void CharacterEntity::OnBegin()
	{
		m_characterComponent = CreateGameObject<CharacterController>();
		m_spriteComponent = CreateGameObject<CharacterGraphics>();


		AddComponent(m_characterComponent);
		AddComponent(m_spriteComponent);
	}

	void CharacterEntity::OnEnd()
	{
		m_characterComponent = nullptr;
		m_spriteComponent = nullptr;
	}

	void CharacterEntity::OnUpdate(Timestep timestep)
	{

	}