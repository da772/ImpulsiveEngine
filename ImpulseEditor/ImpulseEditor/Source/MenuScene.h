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
		textComponent = buttonsEntity->AddComponent<UIComponent>();

		float size = max((float)Application::GetWidth() / (float)Application::GetUIResolutionWidth(), (float)Application::GetHeight() / (float)Application::GetUIResolutionHeight());
		float buttonY = size * ((float)buttonTexture->GetHeight() / (float)Application::GetHeight()) * 5.f;
		float buttonX = size * ((float)buttonTexture->GetWidth() / (float)Application::GetWidth()) * 5.f;

		startButton = buttonsEntity->AddComponent<ButtonComponent>(Vector3f(0, 0, 10), 0.f, Vector2f(buttonX, buttonY), Vector4f(1, 1, 1, 1.f));
		auto reloadButton = buttonsEntity->AddComponent<ButtonComponent>( Vector3f(0, .5, 10), 0.f, Vector2f(buttonX, buttonY), Vector4f(1, 1, 1, 1.f));
		auto refreshButton = buttonsEntity->AddComponent<ButtonComponent>( Vector3f(0, -.5, 10), 0.f, Vector2f(buttonX, buttonY), Vector4f(1, 0, 0, 1.f));
		startButton->SetImageTexture(buttonTexture);
		reloadButton->SetImageTexture(Texture2D::Create("Content/Textures/videoLife_button_7.png", 192));
		refreshButton->SetImageTexture(Texture2D::Create("Content/Textures/videoLife_button_7.png", 192));

		textComponent->CreateQuad({ 0,0,2 }, 0, { 2.5,2,1 }, { 1,1,1,1.f }, Texture2D::Create("Content/Textures/sky_01.png"));
		
		NativeScriptComponent* sc = buttonsEntity->AddComponent<NativeScriptComponent>("TestScript");

		// TEMP FOR ANDROID UNLOADING TEXTURES FIX
		SpriteComponent* s = buttonsEntity->AddComponent<SpriteComponent>();

		//s->CreateQuad({ 0,-1,10 }, 0, { 1 }, { 1 }, Texture2D::Create("Content/Textures/bricks_01.png", 192));

		reloadButton->SetOnEvent([this, reloadButton](const Event& e) {
			if (e.GetEventType() == EventType::MouseButtonReleased) {
#ifdef GE_HOT_RELOAD
				//buttonsEntity->RemoveComponent<NativeScriptComponent>(sc);
				ScriptApi::Load(GEngine::FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET) + "ImpulseEditor/Scripts/CPP/Scripts/", ".h");
#endif
			}
		});


		refreshButton->SetOnEvent([this, &sc](const Event& e) {
			if (e.GetEventType() == EventType::MouseButtonReleased) {
#ifdef GE_HOT_RELOAD
                GEngine::SceneManager::Shutdown();
				//buttonsEntity->RemoveComponent<NativeScriptComponent>(sc);
				ScriptApi::Load(GEngine::FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET) + "ImpulseEditor/Scripts/CPP/Scripts/", ".h");
#endif
				GEngine::SceneManager::SetCurrentScene("menuScene");
			}
            if (e.GetEventType() == EventType::TouchReleased) {
                
            }
			});

		startButton->SetOnEvent([this, sc](const Event& e) {

			if (e.GetEventType() == EventType::MouseButtonReleased) {
				GE_LOG_DEBUG("EVENT: {0}", e.GetName());

				GEngine::SceneManager::SetCurrentScene("menuScene");
			}

			if (e.GetEventType() == EventType::TouchReleased) {
				GE_LOG_DEBUG("EVENT: {0}", e.GetName());
				//GEngine::SceneManager::SetCurrentScene("mainGame");
                GEngine::SceneManager::SetCurrentScene("menuScene");
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