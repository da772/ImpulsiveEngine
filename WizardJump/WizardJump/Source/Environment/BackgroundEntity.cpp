#include "Environment/BackgroundEntity.hpp"

void BackgroundEntity::AddParalaxBackground(const std::string& name, Ref<Texture2D> texture, const glm::vec2& scale, float speed, float zOrder, const glm::vec2& offset)
{
	m_backgrounds[name] = {bInit ? m_backgroundSprite->CreateQuad({ offset.x, offset.y,zOrder }, 0, { scale.x, scale.y, 1 }, { 1,1,1,1 }, texture) : -1, texture, speed, scale, zOrder, offset , bInit };
}

void BackgroundEntity::RemoveParalaxBackground(const std::string& name) {

	if (bInit) {
		
		m_backgroundSprite->RemoveQuad(m_backgrounds[name].id);
		
	}

	m_backgrounds.erase(name);
}

void BackgroundEntity::OnBegin()
{
	m_camera = SceneManager::GetCurrentScene()->GetCamera();
	m_backgroundSprite = CreateGameObject<SpriteComponent>();
	AddComponent(m_backgroundSprite);

	for (const std::pair<std::string, FParalaxBackground>& p : m_backgrounds) {
		if (p.second.bInit) continue;
			m_backgrounds[p.first].id = m_backgroundSprite->CreateQuad({ p.second.offset.x,p.second.offset.y,p.second.zOrder }, 0, { p.second.scale.x, p.second.scale.y, 1 }, { 1,1,1,1 }, p.second.texture.lock());
		
	}

}

void BackgroundEntity::OnEnd()
{
	m_backgroundSprite = nullptr;
}

void BackgroundEntity::OnUpdate(Timestep timestep)
{
	const glm::vec3& camPos = m_camera->GetPosition();
	for (const std::pair<std::string, FParalaxBackground>& p : m_backgrounds) {
		float temp = camPos.x * (1.f - p.second.speed);
		float dist = camPos.x* p.second.speed;
		m_backgroundSprite->SetPosition(p.second.id, { p.second.offset.x + dist,p.second.offset.y});

		if (temp > p.second.offset.x + p.second.scale.x/2.f) m_backgrounds[p.first].offset.x += p.second.scale.x/2.f;
		else if (temp < p.second.offset.x - p.second.scale.x/2.f) m_backgrounds[p.first].offset.x -= p.second.scale.x/2.f;

	}
}
