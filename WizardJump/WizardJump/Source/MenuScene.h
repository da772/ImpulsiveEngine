#pragma once

#include <GEngine.h>

using namespace GEngine;

class MenuScene : public GEngine::Scene {


public:

	inline MenuScene(const char* id, GEngine::Camera* camera) : Scene(id, camera) {


	};
	inline virtual ~MenuScene() {};


	inline void OnUpdate(GEngine::Timestep timestep) override
	{

	}


	inline void OnBegin() override
	{


		camera = m_CameraController->GetCamera().get();
		GEngine::Application::GetApp()->SetTargetCamera(camera);
		buttonsEntity = GEngine::CreateGameObject<GEngine::Entity>();
		AddEntity(buttonsEntity);
		textComponent = GEngine::CreateGameObject<GEngine::UIComponent>();
		buttonsEntity->AddComponent(textComponent);
		textComponent->CreateQuad({ 0,0,0 }, 0, { 1.5,2,1 }, { 1,1,1,1 }, Texture2D::Create("Content/Textures/sky_01.png"));
		Ref<Texture2D> buttonTexture = GEngine::Texture2D::Create("Content/Textures/play_button.png");
		float size = max( (float)Application::GetWidth() /(float)Application::GetUIResolutionWidth(), (float) Application::GetHeight()/(float)Application::GetUIResolutionHeight());
		float buttonY = size*((float)buttonTexture->GetHeight()/ (float)Application::GetHeight())*5.f;
		float buttonX = size*((float)buttonTexture->GetWidth() / (float)Application::GetWidth())*5.f;
		startButton = GEngine::CreateGameObject<GEngine::ButtonComponent>(
			glm::vec3(0, 0, 0), 0.f, glm::vec2(buttonX, buttonY), glm::vec4(1, 1, 1, 1.f));
		buttonsEntity->AddComponent(startButton);
		startButton->SetImageTexture(buttonTexture);
		//FPSuiComponent->CreateText("Ad", font, { .84f, .94f, 1.f }, { .5 ,.5,1 }, { 0,0,0,1 });


		startButton->SetOnEvent([](const Event& e) {

			GE_LOG_DEBUG("EVENT: {0}", e.GetName());

			if (e.GetEventType() == EventType::MouseButtonReleased) {
				GEngine::SceneManager::SetCurrentScene("mainGame");
			}

			if (e.GetEventType() == EventType::TouchReleased) {

				GEngine::SceneManager::SetCurrentScene("mainGame");


			}
			
		});


	
		
	}


	inline void OnEnd() override
	{
		buttonsEntity = nullptr; 
		textComponent = nullptr;
		startButton = nullptr;
	}


	inline void OnLoad() override
	{
		m_CameraController = std::unique_ptr<GEngine::Orthographic_CameraController>(new GEngine::Orthographic_CameraController(
			(float)GEngine::Application::GetApp()->GetWindow()->GetWidth() / (float)GEngine::Application::GetApp()->GetWindow()->GetHeight()));
		
	}


	inline void OnUnload() override
	{
		texture = nullptr;
		m_CameraController = nullptr;
	}

private:
	GEngine::Ref<GEngine::Entity> buttonsEntity = nullptr;
	GEngine::Ref<GEngine::UIComponent> textComponent = nullptr;
	GEngine::Ref<GEngine::ButtonComponent> startButton = nullptr;
	GEngine::Ref<GEngine::Texture2D> texture = nullptr;
	GEngine::Scope<GEngine::Orthographic_CameraController> m_CameraController;

};
