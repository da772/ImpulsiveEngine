#include "Environment/PlatformEntity.hpp"

void PlatformEntity::OnBegin()
{
	SetEntityPosition({ pos.x,pos.y,0 });
	SetEntityScale({ scale.x,scale.y,1 });

	if (debug) {
		m_debugSprite = CreateGameObject<SpriteComponent>();
		AddComponent(m_debugSprite);
		// Bottom
		m_debugSprite->CreateQuad({ 0,-.05f,5 }, rot, { 1,.9f,1 }, { 0,0,0,1.f });
		// Top
		m_debugSprite->CreateQuad({ 0,.45f,6 }, rot, { 1,.1f,.25f }, { 0,1,0,1.f });
	}

	m_colliderWall = CreateGameObject<QuadColliderComponent>(false, true, glm::vec2(0, -.05f), glm::vec2(1, .9f), rot, 0.f);
	m_colliderWall->SetTag("wall");
	AddComponent(m_colliderWall);
	m_colliderWall->SetBounce(.5f);

	m_colliderFloor = CreateGameObject<QuadColliderComponent>(false, true, glm::vec2(0, .45f), glm::vec2(1, .1f), rot, 0.f);
	m_colliderFloor->SetTag("ground");
	AddComponent(m_colliderFloor);

}

void PlatformEntity::OnEnd()
{

}

void PlatformEntity::OnUpdate(Timestep timestep)
{

}
