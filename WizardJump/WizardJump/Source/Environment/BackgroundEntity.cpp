#include "Environment/BackgroundEntity.hpp"

void BackgroundEntity::OnBegin()
{
	m_backgroundSprite = CreateGameObject<SpriteComponent>();
	AddComponent(m_backgroundSprite);
	m_backgroundSprite->CreateQuad({ 0,0,-1 }, 0, { 1000,1000,1 }, { .25,.25,.25,1 }, Texture2D::Create("Content/Textures/Checkerboard.png", TEXTUREFLAGS_Wrap_Repeat), 1000);

}

void BackgroundEntity::OnEnd()
{
	m_backgroundSprite = nullptr;
}

void BackgroundEntity::OnUpdate(Timestep timestep)
{

}
