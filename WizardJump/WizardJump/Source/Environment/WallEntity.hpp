#pragma once



#pragma once
#include "GEngine.h"

using namespace GEngine;


class WallEntity : public Entity {
public:

	inline WallEntity(const Vector2f pos, const Vector2f scale, const float rot = 90) : pos(pos), scale(scale), rot(rot) {

	};
	~WallEntity() {};

	bool debug = true;

protected:
	void OnBegin() override;


	void OnEnd() override;

	Vector2f scale;
	Vector2f pos;
	float rot;

	void OnUpdate(Timestep timestep) override;

	Ref<SpriteComponent> m_debugSprite;
	Ref<QuadColliderComponent> m_collider;

};
