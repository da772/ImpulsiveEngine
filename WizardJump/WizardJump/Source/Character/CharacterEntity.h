#pragma once
#include "GEngine.h"

using namespace GEngine;

class CharacterController;

class CharacterEntity : public Entity {

public:
	CharacterEntity() {};
	~CharacterEntity() {};

protected:
	void OnBegin() override;
	void OnEnd() override;

	void OnUpdate(Timestep timestep) override;

	GEngine::Ref<CharacterController> m_characterComponent;
	GEngine::Ref<SpriteComponent> m_spriteComponent;


};
