#include "Character/CharacterEntity.h"
#include "Character/CharacterController.hpp"
#include "Character/CharacterGraphics.hpp"
#include "Character/CharacterBody.hpp"

CharacterEntity::CharacterEntity(const uint32_t& id) : Entity(id)
{

}


void CharacterEntity::OnBegin()
{

		SetPosition({ m_position.x, m_position.y,0.f });		

		m_characterComponent = AddComponent<CharacterController>(this);
		m_spriteComponent = AddComponent<CharacterGraphics>(this);
		m_audioComponent = AddComponent<AudioListenerComponent>(this);
}

void CharacterEntity::OnEnd()
{
}

void CharacterEntity::OnUpdate(Timestep timestep)
{
}
