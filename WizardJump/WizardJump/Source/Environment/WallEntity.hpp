#pragma once



#pragma once
#include "GEngine.h"

using namespace GEngine;


class WallEntity : public Entity {
public:

	inline WallEntity(const glm::vec2 pos, const glm::vec2 scale, const float rot = 90) : pos(pos), scale(scale), rot(rot) {

	};
	~WallEntity() {};

	bool debug = true;

protected:
	void OnBegin() override;


	void OnEnd() override;

	glm::vec2 scale;
	glm::vec2 pos;
	float rot;

	void OnUpdate(Timestep timestep) override;

	Ref<SpriteComponent> m_debugSprite;
	Ref<QuadColliderComponent> m_collider;

};
