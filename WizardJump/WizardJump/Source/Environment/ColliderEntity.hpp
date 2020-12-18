#pragma once
#include "GEngine.h"

using namespace GEngine;


class ColliderEntity : public Entity {
public:

	ColliderEntity(const Vector3f& pos, const Vector2f& scale, float bounce, const std::string& tag);
	~ColliderEntity() {};

	bool debug = false;

protected:
	void OnBegin() override;


	void OnEnd() override;

	float m_bounce;
	Vector2f m_scale = Vector2f(1,1);
	Vector3f m_pos = Vector3f(0, 0, 0);
	float rot;

	void OnUpdate(Timestep timestep) override;

	Ref<SpriteComponent> m_debugSprite;
	Ref<QuadColliderComponent> m_collider;

};
