#pragma once
#include <GEngine.h>
#include "DebugLayer.h"
#include "Game/GameManager.hpp"
#include "Character/CharacterEntity.h"
#include "Environment/BackgroundEntity.hpp"
#include "Environment/GroundEntity.hpp"
#include "Environment/WallEntity.hpp"
#include "Environment/PlatformEntity.hpp"


class MainGameScene : public GEngine::Scene {


public:

	inline MainGameScene(const char* name, GEngine::Camera* cam) : Scene(name, cam) {


	};

	inline virtual ~MainGameScene() {};

	inline void OnUpdate(GEngine::Timestep timestep) override
	{
		long long time = GEngine::Time::GetEpochTimeMS();

		m_CameraController->SetPosition(e->GetEntityPosition());
		m_CameraController->OnUpdate(timestep);

		

		if (FPSuiComponent) {

			if (textId != 0) {
				FPSuiComponent->RemoveText(textId);
				textId = 0;
			}

			textId = FPSuiComponent->CreateText(std::to_string((int)GEngine::Application::GetApp()->profile["FPS"]) + " FPS", font, { -1, .95f - (((float)GEngine::Application::GetWidth() * .2f) / (float)GEngine::Application::GetHeight())/2.f,1 }, { 0,0,1 }, { 1,1,1,1 });
		}

	}
	
	
	
	inline virtual void OnEvent(GEngine::Event& e) override {
		m_CameraController->OnEvent(e);

	}

	Ref<Entity> e;

	inline void OnBegin() override
	{
		camera = m_CameraController->GetCamera().get();
		GEngine::Application::GetApp()->SetTargetCamera(camera);
		GEngine::Application::GetApp()->SetTargetCameraController(m_CameraController.get());
		GEngine::Application::GetApp()->GetTargetCameraController()->SetCameraZoom(7.5f);
		m_CameraController->SetPosition({ 0,0,0 });
		m_CameraController->SetRotation({ 0,0,0 });

		FPSuiComponent = GEngine::CreateGameObject<GEngine::UIComponent>();

		GEngine::Ref<GEngine::Entity> eFPS = GEngine::CreateGameObject<GEngine::Entity>();
		eFPS->AddComponent(FPSuiComponent);
		AddEntity(GEngine::CreateGameObject<BackgroundEntity>());
		AddEntity(GEngine::CreateGameObject<GameManagerEntity>());
		e = GEngine::CreateGameObject<CharacterEntity>();
		AddEntity(e);

		GEngine::Ref<GEngine::Entity> wall = GEngine::CreateGameObject<WallEntity>(glm::vec2( -20,0), glm::vec2(20,20 ), 90.f);
		AddEntity(wall);
		wall = GEngine::CreateGameObject<WallEntity>(glm::vec2(20, 0), glm::vec2(20, 20), 90.f);
		AddEntity(wall);

		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(4.f, 2.f), glm::vec2(2.f, 1.f)) );
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(8.f, 4.f), glm::vec2(2.f, 1.f)));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(4.f, 6.f), glm::vec2(2.f, 1.f)));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(0.f, 8.f), glm::vec2(2.f, 1.f)));

		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(-2.f, 10.f), glm::vec2(2.f, 1.f)));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(-6.f, 8.f), glm::vec2(2.f, 1.f)));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(-9.5f, 6.f), glm::vec2(2.f, 1.f)));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(-8.f, 4.f), glm::vec2(2.f, 1.f)));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(-6.f, 2.f), glm::vec2(2.f, 1.f)));


		AddEntity(GEngine::CreateGameObject<GroundEntity>());
		AddEntity(eFPS);
        
		/*

#ifdef GE_MOBILE_APP
		GEngine::AdManager::SetUserId("This Is My User ID!");
#ifdef GE_PLATFORM_ANDROID

		GEngine::AdManager::SetAdId("ca-app-pub-0400118858384122~7825957542");
		// Prototype Ad
		GEngine::AdManager::SetRewardAdId("ca-app-pub-4619437690188394/1929986237");

		// Google Test Id
		//GEngine::AdManager::SetRewardAdId("ca-app-pub-3940256099942544/5224354917");
#endif
#ifdef GE_PLATFORM_IOS
		GEngine::AdManager::SetAdId("ca-app-pub-4619437690188394~6799169535");
        // Google Test Id
        //GEngine::AdManager::SetRewardAdId("ca-app-pub-3940256099942544/5224354917");
		GEngine::AdManager::SetRewardAdId("ca-app-pub-4619437690188394/5486087868");

#endif

		GEngine::AdManager::LoadRewardAd([]() {GE_LOG_DEBUG("AD LOADED"); GEngine::AdManager::ShowRewardAd(); },
			[](int i, std::string s) { GE_CORE_DEBUG("AD WATCHED {0} : {1}", i, s); });
#endif
*/
	}



	inline void OnEnd() override
	{

	}

	inline void OnImGuiRender() override {
		
	}

	inline void OnLoad() override
	{
		SetupCamera();

		font = GEngine::Font::Create("Content/Fonts/arial.ttf", (float)GEngine::Application::GetWidth()*.2f );
		font->LoadCharactersEN();

	}

	inline void OnUnload() override
	{
		font = nullptr;
	}

private:

	GEngine::Ref<GEngine::Orthographic_CameraController> m_CameraController;
	GEngine::Ref<GEngine::UIComponent> FPSuiComponent;
	GEngine::Ref<GEngine::Font> font;

	bool bImGui = true;
	long textId = -1;



	inline void SetupCamera() {
		m_CameraController = std::unique_ptr<GEngine::Orthographic_CameraController>(new GEngine::Orthographic_CameraController(
			(float)GEngine::Application::GetApp()->GetWindow()->GetWidth() / (float)GEngine::Application::GetApp()->GetWindow()->GetHeight()));
		m_CameraController->SetOnUpdateFn([this](GEngine::Timestep timeStep, glm::vec3& m_Position, glm::vec3& m_Rotation, glm::vec2& m_LastTouchPos,
			uint64_t& m_lastTouchId, float& m_LastDistance) {

				const float m_CameraMoveSpeed = 1.5f;
				const float m_CameraRotSpeed = 180.f;

				if (GEngine::Input::IsKeyPressed(GE_KEY_A))
					m_Position.x -= m_CameraMoveSpeed * m_CameraController->GetFOV() * timeStep.GetSeconds();
				if (GEngine::Input::IsKeyPressed(GE_KEY_D))
					m_Position.x += m_CameraMoveSpeed * m_CameraController->GetFOV() * timeStep.GetSeconds();
				if (GEngine::Input::IsKeyPressed(GE_KEY_LEFT_SHIFT))
					m_Position.z -= m_CameraMoveSpeed * m_CameraController->GetFOV() * timeStep.GetSeconds();
				if (GEngine::Input::IsKeyPressed(GE_KEY_LEFT_CONTROL))
					m_Position.z += m_CameraMoveSpeed * m_CameraController->GetFOV() * timeStep.GetSeconds();
				if (GEngine::Input::IsKeyPressed(GE_KEY_W))
					m_Position.y += m_CameraMoveSpeed * m_CameraController->GetFOV() * timeStep.GetSeconds();
				if (GEngine::Input::IsKeyPressed(GE_KEY_S))
					m_Position.y -= m_CameraMoveSpeed * m_CameraController->GetFOV() * timeStep.GetSeconds();
				if (GEngine::Input::IsKeyPressed(GE_KEY_Q))
					m_Rotation.z += m_CameraRotSpeed * m_CameraController->GetFOV() * timeStep.GetSeconds();
				if (GEngine::Input::IsKeyPressed(GE_KEY_E))
					m_Rotation.z -= m_CameraRotSpeed * m_CameraController->GetFOV() * timeStep.GetSeconds();
                /*
				if (GEngine::Mobile_Input::GetTouchCount() == 1) {
					std::unordered_map<uint64_t, GEngine::FTouchInfo> touches = GEngine::Mobile_Input::GetTouches();
					if (touches.begin()->first == m_lastTouchId) {
						if (abs(m_LastTouchPos.x - touches.begin()->second.x) > .1f ||
							abs(m_LastTouchPos.y - touches.begin()->second.y) > .1f) {

							glm::vec2 m_touchDir = { m_LastTouchPos.x - touches.begin()->second.x, m_LastTouchPos.y - touches.begin()->second.y };
							m_LastTouchPos = { touches.begin()->second.x, touches.begin()->second.y };
							m_Position.y += (m_CameraMoveSpeed * m_CameraController->GetFOV() * timeStep.GetSeconds()) * -m_touchDir.y / 10.f;
							m_Position.x += (m_CameraMoveSpeed * m_CameraController->GetFOV() * timeStep.GetSeconds()) * m_touchDir.x / 10.f;
						}

					}
				}
                 */
/*
				if (GEngine::Mobile_Input::GetTouchCount() == 2) {
					std::unordered_map<uint64_t, GEngine::FTouchInfo> touches = GEngine::Mobile_Input::GetTouches();
					std::unordered_map<uint64_t, GEngine::FTouchInfo>::iterator it = ++touches.begin();

					if (it->first != m_lastTouchId) {
						float distance = glm::distance(m_LastTouchPos, { it->second.x, it->second.y });
						if (abs(distance - m_LastDistance) > .25f) {
							float d = glm::clamp(distance - m_LastDistance, -1.f, 1.f);
							m_CameraController->OnCameraZoom(0, d, .1f);
							m_LastDistance = distance;
						}

					}


				}

				if (GEngine::Mobile_Input::GetTouchCount() == 0) {
					m_lastTouchId = 0;
				}
				else {
					std::unordered_map<uint64_t, GEngine::FTouchInfo> touches = GEngine::Mobile_Input::GetTouches();
					m_lastTouchId = touches.begin()->first;
					m_LastTouchPos = { touches.begin()->second.x, touches.begin()->second.y };
				}
*/

		});
		m_CameraController->SetOnEventFn([this](GEngine::Event& e) {
			GEngine::EventDispatcher dispatcher(e);
			dispatcher.Dispatch<GEngine::MouseScrolledEvent>([this](GEngine::MouseScrolledEvent& e) {
				m_CameraController->OnCameraZoom(e.GetXOffset(), e.GetYOffset(), .1f);
				return false;
				});
			});
	};

};
