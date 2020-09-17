#include "Character/CharacterController.hpp"

#include "Character/CharacterEntity.h"

const float CharacterController::maxJumpXVel = 5.f;
const float CharacterController::maxJumpYVel = 5.f;
const float CharacterController::jumpXDragClamp = .35f;
const float CharacterController::jumpYDragClamp = .3f;

const float CharacterController::jumpYMultipler = 10.f;
const float CharacterController::jumpXMultipler = 4.f;

void CharacterController::OnBegin()
{
	startTime = Time::GetEpochTimeNS();
	bodyComp = CreateGameObject<CharacterBody>();
	GetEntity()->AddComponent(bodyComp);
	jumpSound = CreateGameObject<AudioComponent>("Content/Audio/testJump01.ogg", false, false, true);
	landSound = CreateGameObject<AudioComponent>("Content/Audio/dink.ogg", false, false, true);
	//musicSound = CreateGameObject<AudioComponent>("Content/Audio/test.ogg", true, true, true, .25f);

	GetEntity()->AddComponent(landSound);
	GetEntity()->AddComponent(jumpSound);
	//GetEntity()->AddComponent(musicSound);
	graphicsComp = static_pointer_cast<CharacterEntity>(GetEntity())->m_spriteComponent;
}


