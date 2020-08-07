#pragma once
#include <GEngine.h>

class Prototype : public GEngine::Application
{

public:
	inline Prototype() {};

	virtual inline void OnCleanDirtyApi() override {};

	virtual inline void OnUpdate(GEngine::Timestep timeStep) override {};

	inline ~Prototype() {};


};


