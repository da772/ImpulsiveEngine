#pragma once

#include <GEngine.h>

using namespace GEngine;

class MenuScene : public GEngine::Scene {


public:
	inline MenuScene(const char* id, GEngine::Camera* camera = nullptr) : Scene(id, camera) {
		m_CameraController = std::unique_ptr<GEngine::Orthographic_CameraController>(new GEngine::Orthographic_CameraController(
			(float)GEngine::Application::GetApp()->GetWindow()->GetWidth() / (float)GEngine::Application::GetApp()->GetWindow()->GetHeight()));
		buttonTexture = GEngine::Texture2D::Create("Content/Textures/play_button.png");
		camera = m_CameraController->GetCamera().get();
		GEngine::Application::GetApp()->SetTargetCamera(camera);

		buttonsEntity = CreateEntity<Entity>();
		textComponent = buttonsEntity->AddComponent<UIComponent>(buttonsEntity);

		float size = max((float)Application::GetWidth() / (float)Application::GetUIResolutionWidth(), (float)Application::GetHeight() / (float)Application::GetUIResolutionHeight());
		float buttonY = size * ((float)buttonTexture->GetHeight() / (float)Application::GetHeight()) * 5.f;
		float buttonX = size * ((float)buttonTexture->GetWidth() / (float)Application::GetWidth()) * 5.f;

		startButton = buttonsEntity->AddComponent<ButtonComponent>(buttonsEntity, Vector3f(0, 0, 10), 0.f, Vector2f(buttonX, buttonY), Vector4f(1, 1, 1, 1.f));
		startButton->SetImageTexture(buttonTexture);

		textComponent->CreateQuad({ 0,0,2 }, 0, { 2.5,2,1 }, { 1,1,1,1.f }, Texture2D::Create("Content/Textures/sky_01.png"));
		
		buttonsEntity->AddComponent<NativeScriptComponent>(buttonsEntity, "TestScript");

		// TEMP FOR ANDROID UNLOADING TEXTURES FIX
		SpriteComponent* s = buttonsEntity->AddComponent<SpriteComponent>(buttonsEntity);

		//s->CreateQuad({ 0,-1,10 }, 0, { 1 }, { 1 }, Texture2D::Create("Content/Textures/bricks_01.png", 192));

		startButton->SetOnEvent([](const Event& e) {

			if (e.GetEventType() == EventType::MouseButtonReleased) {
				GE_LOG_DEBUG("EVENT: {0}", e.GetName());
				GEngine::SceneManager::SetCurrentScene("menuScene");
			}

			if (e.GetEventType() == EventType::TouchReleased) {

				GE_LOG_DEBUG("EVENT: {0}", e.GetName());
				//GEngine::SceneManager::SetCurrentScene("mainGame");


			}

			});

	};
	inline virtual ~MenuScene() {
	};


	inline void OnUpdate(GEngine::Timestep timestep) override
	{

	}


	inline void OnBegin() override
	{

	
	}


	inline void OnEnd() override
	{

	}


	inline void OnLoad() override
	{

	}


	inline void OnUnload() override
	{

	}

private:
	GEngine::Entity* buttonsEntity = nullptr;
	GEngine::UIComponent* textComponent = nullptr;
	GEngine::ButtonComponent* startButton = nullptr;
	Ref<GEngine::Texture2D> buttonTexture = nullptr;
	GEngine::Scope<GEngine::Orthographic_CameraController> m_CameraController;

};
