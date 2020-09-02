#include "Environment/WallEntity.hpp"

void WallEntity::OnBegin()
{
	SetEntityPosition({ pos.x,pos.y,0 });
	SetEntityScale({ scale.x,scale.y,1 });

	if (debug) {
		m_debugSprite = CreateGameObject<SpriteComponent>();
		AddComponent(m_debugSprite);
		m_debugSprite->CreateQuad({ 0,0,0 }, rot, { 1,1,1 }, { 1.f,1.f,1.f,1.f }, 
                                  Texture2D::Create("Content/Textures/bluishBricks.png", TEXTUREFLAGS_Wrap_Repeat), { scale.x/4.f, scale.y/4.f});
	}

	m_collider = CreateGameObject<QuadColliderComponent>(false, true, glm::vec2(0, 0), glm::vec2(1, 1), rot, 0.f);
	m_collider->SetTag("wall");
	AddComponent(m_collider);
	m_collider->SetBounce(.5f);
}

void WallEntity::OnEnd()
{

}

void WallEntity::OnUpdate(Timestep timestep)
{

}
