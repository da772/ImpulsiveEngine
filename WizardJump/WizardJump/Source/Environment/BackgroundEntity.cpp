#include "Environment/BackgroundEntity.hpp"

void BackgroundEntity::OnBegin()
{
	m_backgroundSprite = CreateGameObject<SpriteComponent>();
	AddComponent(m_backgroundSprite);
	m_backgroundSprite->CreateQuad({ 0,0,-200 }, 0, { 1000,1000,1 }, {.75f, .75f, .75f, 1},//{ 181.f/255.f,244.f/255.f,249.f/255.f, 1 },
		GEngine::Texture2D::Create("Content/Textures/bricks-center.png", TEXTUREFLAGS_Wrap_Repeat | TEXTUREFLAGS_DisableMipMap), {1000, 1000});
	//m_backgroundSprite->CreateQuad({ 0,60,-100 }, 0, { 33.75f, 120, 1 }, { 1,1,1,1 }, , { 6,18 });
	//m_backgroundSprite->CreateQuad({ 5.f, 7.7f,-50 }, 0, { 6.f, 18.f, 1 }, { 1,1,1,1 }, GEngine::Texture2D::Create("Content/Textures/bricks-center.png", TEXTUREFLAGS_Wrap_Repeat | TEXTUREFLAGS_DisableMipMap), { 6,18 });
}

void BackgroundEntity::OnEnd()
{
	m_backgroundSprite = nullptr;
}

void BackgroundEntity::OnUpdate(Timestep timestep)
{

}
