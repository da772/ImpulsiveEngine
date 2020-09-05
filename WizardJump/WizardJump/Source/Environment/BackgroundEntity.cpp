#include "Environment/BackgroundEntity.hpp"

void BackgroundEntity::OnBegin()
{
	m_backgroundSprite = CreateGameObject<SpriteComponent>();
	AddComponent(m_backgroundSprite);
	m_backgroundSprite->CreateQuad({ 0,0,-1 }, 0, { 1000,1000,1 }, { 38.f/255.f, 56.f/255.f, 72.f/255.f, 1 },
		nullptr, {1, 1});

}

void BackgroundEntity::OnEnd()
{
	m_backgroundSprite = nullptr;
}

void BackgroundEntity::OnUpdate(Timestep timestep)
{

}
