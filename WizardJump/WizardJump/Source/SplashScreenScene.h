#pragma once

#include <GEngine.h>

class SplashScreenScene : public GEngine::Scene {


public:
	
	inline SplashScreenScene(const char* id, GEngine::Camera* camera) : Scene(id, camera) {
		

	};
	inline virtual ~SplashScreenScene() {};

	bool m_sceneChanged = false;

	inline void OnEvent(GEngine::Event& e) override {

		if (e.GetEventType() == GEngine::EventType::KeyPressed) {
			if (!m_sceneChanged) {
				GEngine::SceneManager::SetCurrentScene("mainGame");
				m_sceneChanged = true;
			}

		}

		if (e.GetEventType() == GEngine::EventType::MouseButtonPressed) {
			if (!m_sceneChanged) {
				GEngine::SceneManager::SetCurrentScene("mainGame");
				m_sceneChanged = true;
			}

		}

		if (e.GetEventType() == GEngine::EventType::TouchPressed) {
			if (!m_sceneChanged) {
				GEngine::SceneManager::SetCurrentScene("mainGame");
				m_sceneChanged = true;
			}
		}
	}

	inline void OnUpdate(GEngine::Timestep timestep) override
	{
		
	}


	inline void OnBegin() override
	{
		camera = m_CameraController->GetCamera().get();
		GEngine::Application::GetApp()->SetTargetCamera(camera);
		logoEntity = GEngine::CreateGameObject<GEngine::Entity>();
		logoComponent = GEngine::CreateGameObject<GEngine::UIComponent>();
		logoAnimation = GEngine::CreateGameObject<GEngine::SpriteAnimationComponent>();
		logoEntity->AddComponent(logoComponent);
		logoEntity->AddComponent(logoAnimation);

		int width, height;
		GEngine::Application::GetApp()->GetWindow()->GetFrameBufferSize(&width, &height);

		float x = width > height ? 1 : (float)width / (float)height;
		float y = height > width ? 1 : (float)height / (float)width;

		long id = logoComponent->CreateQuad({ 0,0,0 }, { 0 }, { 1.f * y,1.f * x,1 }, { 1,1,1,0.f }, texture);
		logoComponent->CreateQuad({ 0,0,-1 }, { 0 }, { 2,2,1 }, { .05f,.05f,.05f,1 });
		
		logoAnimation->SetFrameAnimation(30, 150, false, [this, id](int frame) {
			logoComponent->SetColor(id, { 1, 1, 1, frame / 150.f });
			if (frame == 20) {

			}
			if (frame >= 150) {
				if (GEngine::SceneManager::GetScene("mainGame")->IsLoaded()) {
					GEngine::SceneManager::SetCurrentScene("mainGame");
				}
				else {
                    std::lock_guard<std::mutex> guard(loadNewScene);
					bSceneLoaded = true;
				}
			}
		});
		

		AddEntity(logoEntity);
		GEngine::SceneManager::GetScene("mainGame")->Load();	
	}


	inline void OnEnd() override
	{
		m_sceneChanged = false;
	}


	inline void OnLoad() override
	{
		m_CameraController = std::unique_ptr<GEngine::Orthographic_CameraController>(new GEngine::Orthographic_CameraController(
			(float)GEngine::Application::GetApp()->GetWindow()->GetWidth() / (float)GEngine::Application::GetApp()->GetWindow()->GetHeight()));
		texture = GEngine::Texture2D::Create("Content/Textures/ImpulsiveGamesLogo.png", TEXTUREFLAGS_Mag_Nearest);
	}


	inline void OnUnload() override
	{
		texture = nullptr;
		m_CameraController = nullptr;
	}

private:
	GEngine::Ref<GEngine::Entity> logoEntity = nullptr;
	GEngine::Ref<GEngine::UIComponent> logoComponent = nullptr;
	GEngine::Ref<GEngine::SpriteAnimationComponent> logoAnimation = nullptr;
	GEngine::Ref<GEngine::Texture2D> texture = nullptr;
	GEngine::Scope<GEngine::Orthographic_CameraController> m_CameraController;
	std::mutex loadNewScene;
	std::condition_variable condition;
	bool bSceneLoaded = false;

};
