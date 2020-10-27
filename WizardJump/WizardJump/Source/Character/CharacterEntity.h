#pragma once
#include "GEngine.h"

using namespace GEngine;

class CharacterController;
class CharacterGraphics;


class CharacterEntity : public Entity {

public:
	CharacterEntity(const glm::vec2& position = glm::vec2(0,0));
	~CharacterEntity() {};


	GEngine::Ref<CharacterController> m_characterComponent;
	GEngine::Ref<CharacterGraphics> m_spriteComponent;
	GEngine::Ref<AudioListenerComponent> m_audioComponent;


protected:
	void OnBegin() override;
	void OnEnd() override;
	glm::vec2 m_position;

	void OnUpdate(Timestep timestep) override;



};
