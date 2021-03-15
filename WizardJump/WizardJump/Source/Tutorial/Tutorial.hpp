#pragma once
#include <GEngine.h>
#if 0
using namespace GEngine;
class CharacterEntity;


class Tutorial : public Entity {
public:
	inline Tutorial() { go_tag = "tutorial"; };
	inline ~Tutorial() {};

	static void CreateMainTutorial(Weak<Scene> s, Weak<CharacterEntity> characterEntity);

protected:
	inline virtual void OnBegin() override {};
	inline virtual void OnUpdate(Timestep t) override {};
	inline virtual void OnEnd() override {};


};

#endif

