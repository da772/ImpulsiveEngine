#pragma once
#include "GEngine.h"

using namespace GEngine;


class GroundEntity : public Entity {
public:

	GroundEntity() {};
	~GroundEntity() {};

	bool debug = true;

protected:
	void OnBegin() override;


	void OnEnd() override;


	void OnUpdate(Timestep timestep) override;

	Ref<SpriteComponent> m_debugSprite;
	Ref<QuadColliderComponent> m_collider;

};
