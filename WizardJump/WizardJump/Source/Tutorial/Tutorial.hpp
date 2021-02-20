#pragma once
#include <GEngine.h>

using namespace GEngine;
class CharacterEntity;


class Tutorial : public Entity {
public:
	inline Tutorial() { m_tag = "tutorial"; };
	inline ~Tutorial() {};

	static void CreateMainTutorial(Weak<Scene> s, Weak<CharacterEntity> characterEntity);

protected:
	inline virtual void OnBegin() override {};
	inline virtual void OnUpdate(Timestep t) override {};
	inline virtual void OnEnd() override {};


};


