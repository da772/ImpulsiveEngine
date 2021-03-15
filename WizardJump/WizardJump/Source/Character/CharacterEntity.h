#pragma once
#include "GEngine.h"

using namespace GEngine;

class CharacterController;
class CharacterGraphics;


class CharacterEntity : public Entity {

public:
	CharacterEntity(const uint32_t& id);
	~CharacterEntity() {};


	CharacterController* m_characterComponent;
	CharacterGraphics* m_spriteComponent;
	AudioListenerComponent* m_audioComponent;


protected:
	void OnBegin() override;
	void OnEnd() override;
	Vector2f m_position;

	void OnUpdate(Timestep timestep) override;



};
