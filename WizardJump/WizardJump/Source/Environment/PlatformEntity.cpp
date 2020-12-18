#include "Environment/PlatformEntity.hpp"


PlatformEntity::PlatformEntity(const Vector2f pos, const Vector2f scale /*= Vector2f(1, 1)*/, const float zOrder /*= 1*/, const std::string& entityTag /*= ""*/, const float rot /*= 0*/) : pos(pos), scale(scale), rot(rot), zOrder(zOrder) 
{
	if (entityTag.size() > 0)
		m_tag = entityTag;
}

void PlatformEntity::OnBegin()
{
	SetEntityPosition({ pos.x,pos.y,0 });
	SetEntityScale({ .5f,.5f,1 });


	

	m_sprite = CreateGameObject<SpriteComponent>();
	AddComponent(m_sprite);

	m_sprite->CreateSubTexturedQuad({ 0,0,zOrder }, 0, { 1,1,1 }, { 1,1,1,1 }, SubTexture2D::CreateFromCoords(Texture2D::Create("Content/Textures/woodPlatforms.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest), { 0, 2 }, { 16,16 }, { 1,1 }));
	int count = 0;
	for (float i = .5f; i < scale.x/2.f; i+= .5f) {
		count++;
		m_sprite->CreateSubTexturedQuad({ i,0,zOrder }, 0, { 1,1,1 }, { 1,1,1,1 }, SubTexture2D::CreateFromCoords(Texture2D::Create("Content/Textures/woodPlatforms.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest), { (float)1+count%4, 2.f }, GEngine::Vector2f( 16,16 ), { 1,1 }));
	}
	m_sprite->CreateSubTexturedQuad({ scale.x/2.f,0,zOrder }, 0, { 1,1,1 }, { 1,1,1,1 }, SubTexture2D::CreateFromCoords(Texture2D::Create("Content/Textures/woodPlatforms.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest), { 5, 2 }, { 16,16 }, { 1,1 }));

}

void PlatformEntity::OnEnd()
{
	m_debugSprite = nullptr;
	m_sprite = nullptr;
}

void PlatformEntity::OnUpdate(Timestep timestep)
{

}
