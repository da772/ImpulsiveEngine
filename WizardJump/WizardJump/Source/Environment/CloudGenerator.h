#pragma once

#include "GEngine.h"

using namespace GEngine;

static constexpr int CLOUD_GENERATOR_CLOUD_COUNT = 8;


class CloudGenerator : public Entity {


public:
	CloudGenerator(const Vector3f& startPosition, const Vector3f& endPosition, const Vector2f& heightMinMax, const float& cloudSpeed, const int& cloudAmt);

	~CloudGenerator();


private:
	Vector3f m_startPosition, m_endPosition;
	Vector2f m_heights;
	float m_cloudSpeed;
	int m_cloudAmt;

	Ref<SpriteComponent> m_spriteComponent;

	Ref<Texture2D> m_cloudTextures[CLOUD_GENERATOR_CLOUD_COUNT];

	ShapeID m_clouds[CLOUD_GENERATOR_CLOUD_COUNT];
	Vector3f m_cloudPos[CLOUD_GENERATOR_CLOUD_COUNT];




protected:
	void OnBegin() override;


	void OnEnd() override;


	void OnUpdate(Timestep timestep) override;

};



