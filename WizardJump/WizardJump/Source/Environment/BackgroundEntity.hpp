#pragma once

#include "GEngine.h"

using namespace GEngine;


class BackgroundEntity : public Entity {

public:
	BackgroundEntity() {};
	~BackgroundEntity() {};

protected:
	void OnBegin() override;


	void OnEnd() override;


	void OnUpdate(Timestep timestep) override;

	Ref<SpriteComponent> m_backgroundSprite;


};