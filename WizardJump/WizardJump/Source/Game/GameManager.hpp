#pragma once
#include "GEngine.h"

using namespace GEngine;


class GameManagerComponent : public Component {
public:
	GameManagerComponent() {
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

class GameManagerEntity : public Entity {
public:
	GameManagerEntity() {
		bUpdates = true;
	};
	~GameManagerEntity() {};
	

protected:
	inline void OnBegin() override
	{
		gameManager = CreateGameObject<GameManagerComponent>();
		AddComponent(gameManager);
	}

	void OnUpdate(Timestep timestep) override
	{

	}

	inline void OnEnd() override
	{
		gameManager = nullptr;
	}

	

private:
	Ref<GameManagerComponent> gameManager;

};

