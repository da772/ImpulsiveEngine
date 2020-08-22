#pragma once
#pragma once
#include "GEngine.h"

using namespace GEngine;


class PlatformEntity : public Entity {
public:

	inline PlatformEntity(const glm::vec2 pos, const glm::vec2 scale = glm::vec2(1,1), const float rot = 0) : pos(pos), scale(scale), rot(rot) {};
	~PlatformEntity() {};

	bool debug = true;

protected:
	void OnBegin() override;


	void OnEnd() override;


	void OnUpdate(Timestep timestep) override;

	glm::vec2 scale;
	glm::vec2 pos;
	float rot;

	Ref<SpriteComponent> m_debugSprite;
	Ref<QuadColliderComponent> m_colliderWall;
	Ref<QuadColliderComponent> m_colliderFloor;

};
