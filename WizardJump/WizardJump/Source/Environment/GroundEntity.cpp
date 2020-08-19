#include "Environment/GroundEntity.hpp"

void GroundEntity::OnBegin()
{
	SetEntityPosition({ 0,-4,0 });
	SetEntityScale({ 4,1,1 });

	if (debug) {
		m_debugSprite = CreateGameObject<SpriteComponent>();
		AddComponent(m_debugSprite);
		m_debugSprite->CreateQuad({ 0,0,0 }, 0, { 1,1,1 }, { 1,0,0,1 });
	}

	m_collider = CreateGameObject<QuadColliderComponent>(false, true, glm::vec2(0, 0), glm::vec2(1, 1), 0.f, 0.f);
	AddComponent(m_collider);


}

void GroundEntity::OnEnd()
{

}

void GroundEntity::OnUpdate(Timestep timestep)
{

}
