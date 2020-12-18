#pragma once
#include "GEngine.h"

using namespace GEngine;

class CharacterController;
class CharacterGraphics;


class CharacterEntity : public Entity {

public:
	CharacterEntity(const Vector2f& position = Vector2f(0,0));
	~CharacterEntity() {};


	GEngine::Ref<CharacterController> m_characterComponent;
	GEngine::Ref<CharacterGraphics> m_spriteComponent;
	GEngine::Ref<AudioListenerComponent> m_audioComponent;


protected:
	void OnBegin() override;
	void OnEnd() override;
	Vector2f m_position;

	void OnUpdate(Timestep timestep) override;



};
