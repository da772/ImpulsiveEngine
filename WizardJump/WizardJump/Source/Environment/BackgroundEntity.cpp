#include "Environment/BackgroundEntity.hpp"

void BackgroundEntity::AddParalaxBackground(const std::string& name, Ref<Texture2D> texture, const Vector2f& scale, float speed, float zOrder, const Vector2f& offset)
{
	m_backgrounds[name] = { bInit ? m_backgroundSprite->CreateQuad({ offset.x, offset.y,zOrder }, 0, { scale.x, scale.y, 1 }, { 1,1,1,1 }, texture) : -1, texture, speed, scale, zOrder, offset , {0,0}, bInit };
	
}

void BackgroundEntity::RemoveParalaxBackground(const std::string& name) {

	if (bInit) {
		
		m_backgroundSprite->RemoveQuad(m_backgrounds[name].id);
		
	}

	m_backgrounds.erase(name);
}

void BackgroundEntity::OnBegin()
{

	if (Renderer::GetPipeline("background") == nullptr) {
		Ref<RenderPipeline> pipeline = std::shared_ptr<RenderPipeline>(new RenderPipeline_2d(Renderer::GetRenderScale(), "Renderpipeline_2d_Background"));
		Renderer::AddPipeline("background", pipeline, 0);
		pipeline->SetSize(GEngine::Application::GetWidth(), GEngine::Application::GetHeight());
	}

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


		for (const std::pair<std::string, FParalaxBackground>& p : m_backgrounds) {

			float dist = m_camera->GetPosition().x * p.second.speed;
			float xPos = p.second.pos.x + p.second.offset.x + dist;
			m_backgroundSprite->SetPosition(p.second.id, { xPos,p.second.offset.y });

			if (m_camera->GetPosition().x > xPos + p.second.scale.x) {
				m_backgrounds[p.first].offset.x += p.second.scale.x * 2;
			}
			else if (m_camera->GetPosition().x < xPos - (p.second.scale.x)) {
				m_backgrounds[p.first].offset.x -= p.second.scale.x * 2;
			}

		}
}
