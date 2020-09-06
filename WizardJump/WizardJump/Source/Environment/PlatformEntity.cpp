#include "Environment/PlatformEntity.hpp"

void PlatformEntity::OnBegin()
{
	SetEntityPosition({ pos.x,pos.y,0 });
	SetEntityScale({ scale.x,scale.y,1 });

	if (debug) {
		m_debugSprite = CreateGameObject<SpriteComponent>();
		AddComponent(m_debugSprite);
		// Bottom
		m_debugSprite->CreateQuad({ 0, scale.y*-.025f,5 }, rot, { 1,.95f,1 }, { 0,0,0,.45f });
		// Top
		m_debugSprite->CreateQuad({ 0, scale.y*.475f,6 }, rot, { 1,.05f,1.f }, { 0,1,0,.45f });


		// Test Texture

		
	}

	m_sprite = CreateGameObject<SpriteComponent>();
	AddComponent(m_sprite);
	m_sprite->CreateSubTexturedQuad({ 0, 0,0 }, rot, { 1,1,1 }, { 1,1,1,1.f },
		SubTexture2D::CreateFromCoords(Texture2D::Create("Content/Textures/bluishBricks.png", TEXTUREFLAGS_Wrap_Repeat), { 0,0 }, { 64,64 }, { 1,1 }), { scale.x / 4.f, scale.y / 4.f });


	m_colliderWall = CreateGameObject<QuadColliderComponent>(false, true, glm::vec2(0, scale.y * -.025f), glm::vec2(1, .95f), rot, 0.f);
	m_colliderWall->SetTag("wall");
	AddComponent(m_colliderWall);
	m_colliderWall->SetBounce(.5f);
	

	
	m_colliderFloor = CreateGameObject<QuadColliderComponent>(false, true, glm::vec2(0, scale.y * .475f), glm::vec2(1, .05f), rot, 0.f);
	m_colliderFloor->SetTag("ground");
	AddComponent(m_colliderFloor);


}

void PlatformEntity::OnEnd()
{
	m_debugSprite = nullptr;
	m_colliderFloor = nullptr;
	m_colliderWall = nullptr;
	m_sprite = nullptr;
}

void PlatformEntity::OnUpdate(Timestep timestep)
{

}
