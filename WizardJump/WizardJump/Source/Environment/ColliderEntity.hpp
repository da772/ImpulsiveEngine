#pragma once
#include "GEngine.h"

using namespace GEngine;


class ColliderEntity : public Entity {
public:

	ColliderEntity(const glm::vec3& pos, const glm::vec2& scale, float bounce, const std::string& tag);
	~ColliderEntity() {};

	bool debug = false;

protected:
	void OnBegin() override;


	void OnEnd() override;

	float m_bounce;
	glm::vec2 m_scale = glm::vec2(1,1);
	glm::vec3 m_pos = glm::vec3(0, 0, 0);
	float rot;

	void OnUpdate(Timestep timestep) override;

	Ref<SpriteComponent> m_debugSprite;
	Ref<QuadColliderComponent> m_collider;

};
