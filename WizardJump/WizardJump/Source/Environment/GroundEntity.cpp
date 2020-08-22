#include "Environment/GroundEntity.hpp"

void GroundEntity::OnBegin()
{
	SetEntityPosition({ 0,-10,0 });
	SetEntityScale({ 20,20,1 });

	if (debug) {
		m_debugSprite = CreateGameObject<SpriteComponent>();
		AddComponent(m_debugSprite);
		m_debugSprite->CreateQuad({ 0,0,0 }, 0, { 1,1,1 }, { .0f,1.f,0.f,1.f }, Texture2D::Create("Content/Textures/Checkerboard.png", TEXTUREFLAGS_Wrap_Repeat), 20.f);
	}

	m_collider = CreateGameObject<QuadColliderComponent>(false, true, glm::vec2(0, 0), glm::vec2(1, 1), 0.f, 0.f);
	m_collider->SetTag("ground");
	AddComponent(m_collider);


}

void GroundEntity::OnEnd()
{

}

void GroundEntity::OnUpdate(Timestep timestep)
{

}
