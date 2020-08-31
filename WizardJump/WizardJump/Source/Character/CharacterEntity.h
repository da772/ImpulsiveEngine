#pragma once
#include "GEngine.h"

using namespace GEngine;

class CharacterController;
class CharacterGraphics;


class CharacterEntity : public Entity {

public:
	CharacterEntity() {};
	~CharacterEntity() {};


	GEngine::Ref<CharacterController> m_characterComponent;
	GEngine::Ref<CharacterGraphics> m_spriteComponent;
	GEngine::Ref<AudioListenerComponent> m_audioComponent;


protected:
	void OnBegin() override;
	void OnEnd() override;

	void OnUpdate(Timestep timestep) override;



};
