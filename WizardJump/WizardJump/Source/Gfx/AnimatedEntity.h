#pragma once
#include <GEngine.h>

using namespace GEngine;


class AnimatedEntity : public Entity {


public:
	AnimatedEntity(Ref<SubTexture2D> texture, const Vector2f& scale, const Vector4f& color, uint32_t rowSize, uint32_t colSize, uint8_t fps, uint32_t maxFrames, bool loop );
	~AnimatedEntity();

	inline Ref<SubTexture2D> GetTexture() const { return m_texture; }

private:
	Ref<SpriteComponent> m_spriteComponent;
	Ref<SpriteAnimationComponent> m_animatedComp;
	Ref<SubTexture2D> m_texture;
	uint32_t m_rowSize, m_colSize, m_fps, m_maxFrames;
	bool m_loop;
	Vector2f m_startCoords;
	ShapeID m_id;
	Vector2f m_scale;
	Vector4f m_color;

protected:
	void OnBegin() override;

	void OnEnd() override;

};