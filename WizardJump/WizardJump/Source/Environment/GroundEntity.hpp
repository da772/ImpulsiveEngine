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


	glm::vec2 scale = glm::vec2(40,20);
	glm::vec2 pos = glm::vec2(0, -10);
	float rot;

	void OnUpdate(Timestep timestep) override;

	Ref<SpriteComponent> m_debugSprite;
	Ref<QuadColliderComponent> m_collider;

};
