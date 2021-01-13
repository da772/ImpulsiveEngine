#include "CloudGenerator.h"

CloudGenerator::CloudGenerator(const Vector3f& startPosition, const Vector3f& endPosition, const Vector2f& heightMinMax, const float& cloudSpeed, const int& cloudAmt) : Entity(),
	m_startPosition(startPosition), m_endPosition(endPosition), m_heights(heightMinMax), m_cloudSpeed(cloudSpeed), m_cloudAmt(cloudAmt)
{
	bUpdates = true;



}

CloudGenerator::~CloudGenerator()
{

}

void CloudGenerator::OnBegin()
{

	m_spriteComponent = CreateGameObject<SpriteComponent>();
	AddComponent(m_spriteComponent);

	for (int i = 0; i < CLOUD_GENERATOR_CLOUD_COUNT; i++) {
		m_cloudTextures[i] = Texture2D::Create("Content/Textures/Clouds/cloud" + std::to_string(i + 1)+".png", TEXTUREFLAGS_Min_Linear | TEXTUREFLAGS_Min_Nearest);
		Vector2f scale((float)m_cloudTextures[i]->GetWidth() / 32.f, (float)m_cloudTextures[i]->GetHeight() / 32.f);
		m_cloudPos[i] = { m_startPosition + (scale*i), m_cloudPos[i].z };
		m_cloudPos[i].y += Random::FloatRange(m_heights.x, m_heights.y);
		m_clouds[i] = m_spriteComponent->CreateQuad(m_cloudPos[i], 0, { scale, 1.f }, { Vector3f(1.f), .5f }, m_cloudTextures[i]);
	}
	
}

void CloudGenerator::OnEnd()
{
	
}

void CloudGenerator::OnUpdate(Timestep timestep)
{
	for (int i = 0; i < CLOUD_GENERATOR_CLOUD_COUNT; i++) {
		m_cloudPos[i] = Vector3f(Vector2f::lerp(m_cloudPos[i].xy(), { m_cloudPos[i].x - m_cloudSpeed, m_cloudPos[i].y }, timestep), m_cloudPos[i].z);
		if (m_cloudPos[i].x <= m_endPosition.x) {
			m_cloudPos[i] = m_startPosition;
		}
		m_spriteComponent->SetPosition(m_clouds[i], m_cloudPos[i].xy());
	}
}
