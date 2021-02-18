#pragma once
#include <GEngine.h>

using namespace GEngine;
class CharacterEntity;


class Tutorial : public Entity {
public:
	inline Tutorial() { m_tag = "tutorial"; };
	inline ~Tutorial() {};

	static void CreateMainTutorial(Ref<Scene> s, Ref<CharacterEntity> characterEntity);

protected:
	inline virtual void OnBegin() override {};
	inline virtual void OnUpdate(Timestep t) override {};
	inline virtual void OnEnd() override {};


};


