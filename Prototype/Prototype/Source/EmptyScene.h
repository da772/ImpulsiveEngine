#pragma once

#include <GEngine.h>

class EmptyScene : public GEngine::Scene {


public:

	inline EmptyScene(const char* id, GEngine::Camera* camera) : Scene(id, camera) {


	};

	inline virtual ~EmptyScene() {};

	bool m_sceneChanged = false;

	inline void OnEvent(GEngine::Event& e) override {

	
	}

	inline void OnUpdate(GEngine::Timestep timestep) override
	{

	}


	inline void OnBegin() override
	{
		
	}


	inline void OnEnd() override
	{
	}


	inline void OnLoad() override
	{
	
	}


	inline void OnUnload() override
	{
		
	}

private:
	

};
