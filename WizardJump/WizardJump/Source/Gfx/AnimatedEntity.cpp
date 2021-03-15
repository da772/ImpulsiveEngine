#include "AnimatedEntity.h"

AnimatedEntity::AnimatedEntity(Ref<SubTexture2D> texture, const Vector2f& scale, const Vector4f& color, uint32_t rowSize, uint32_t colSize, uint8_t fps, uint32_t maxFrames, bool loop) : m_texture(texture), m_rowSize(rowSize), m_colSize(colSize),
m_fps(fps), m_maxFrames(maxFrames), m_loop(loop), m_scale(scale), m_color(color)
{
	bUpdates = true;
}

AnimatedEntity::~AnimatedEntity()
{
	
}

void AnimatedEntity::OnBegin()
{
	m_spriteComponent = CreateGameObject<SpriteComponent>();
	AddComponent(m_spriteComponent);
	m_animatedComp = CreateGameObject<SpriteAnimationComponent>();
	AddComponent(m_animatedComp);

	m_startCoords = m_texture->m_coords;
	m_id = m_spriteComponent->CreateSubTexturedQuad({ 0,0,0 }, 0, { m_scale, 1.f }, m_color, m_texture);

	m_animatedComp->SetFrameAnimation(m_fps, m_maxFrames, m_loop, [this](int frame) {

		float xPos = m_startCoords.x + frame % m_colSize;
		float yPos = m_startCoords.y + frame % m_rowSize;

		m_texture->SetCoords({ xPos, yPos }, m_texture->m_cellSize, m_texture->m_spriteSize);
	});

}

void AnimatedEntity::OnEnd()
{
	
}
