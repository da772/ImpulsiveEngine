#include "Environment/ColliderEntity.hpp"

ColliderEntity::ColliderEntity(const glm::vec3& pos, const glm::vec2& scale, float bounce, const std::string& tag) : m_pos(pos), m_scale(scale)
{
	m_tag = tag;
	m_bounce = bounce;
}

void ColliderEntity::OnBegin()
{
	SetEntityPosition({ m_pos.x, m_pos.y,0 });
	SetEntityScale({ m_scale.x,m_scale.y,1 });

	if (debug) {
		m_debugSprite = CreateGameObject<SpriteComponent>();
		AddComponent(m_debugSprite);
		m_debugSprite->CreateQuad({ 0,0,20 }, 0, { 1,1,1 }, { 1.f,0.f,0.f,.5f });
	}

	m_collider = CreateGameObject<QuadColliderComponent>(false, true, glm::vec2(m_pos.x, m_pos.y));
	AddComponent(m_collider);
	ColliderID id = m_collider->CreateQuad({ 0,0 }, m_scale, 0, 0, m_tag);
	m_collider->SetBounce(id, m_bounce);


}

void ColliderEntity::OnEnd()
{

}

void ColliderEntity::OnUpdate(Timestep timestep)
{

}
