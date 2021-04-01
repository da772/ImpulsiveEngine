#pragma once
#include "GEngine.h"

using namespace GEngine;


class GameManagerComponent : public Component {
public:
	GameManagerComponent(Entity* e) : Component(e) {
		bUpdates = false;
	};
	~GameManagerComponent() {};



protected:
	void OnBegin() override
	{
		GE_LOG_INFO("GAME STARTED");
	}


	void OnEnd() override
	{
		
	}


	void OnUpdate(Timestep timestep) override
	{
		GE_LOG_INFO("UPDAINTG?");
	}

	

};



