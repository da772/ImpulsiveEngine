#pragma once
#include <GEngine.h>
#include "DebugLayer.h"
#include "Game/GameManager.hpp"
#include "Character/CharacterEntity.h"
#include "Environment/BackgroundEntity.hpp"
#include "Environment/GroundEntity.hpp"
#include "Environment/WallEntity.hpp"
#include "Environment/PlatformEntity.hpp"
#include "Environment/Objects/FireEntity.hpp"
#include "Character/CharacterController.hpp"
#include "Generation/ProceduralPlatformGeneration.hpp"
#include "Environment/BackgroundTiledEntity.hpp"



#include "Lighting/LightComponent.h"


class MainGameScene : public GEngine::Scene {


public:

	inline MainGameScene(const char* name, GEngine::Camera* cam) : Scene(name, cam) {


	};

	inline virtual ~MainGameScene() {};

	inline void OnUpdate(GEngine::Timestep timestep) override
	{
		long long time = GEngine::Time::GetEpochTimeMS();
		m_CameraController->OnUpdate(timestep);
		glm::vec3 pos;
		float dist = GEngine::GEMath::distance(m_CameraController->GetPosition(), characterEntity->GetEntityPosition());
		if (dist > .01f) {
			pos = GEngine::GEMath::lerp(m_CameraController->GetPosition(), characterEntity->GetEntityPosition(), 10.f * timestep);
		}
		else {
			pos = characterEntity->GetEntityPosition();
		}
	
		m_CameraController->SetPosition(pos);

		if (FPSuiComponent && FPSuiComponent->IsInitialized()) {

			if (!textId.empty()) {
				FPSuiComponent->RemoveText(textId);
				textId = "";
			}
            
			textId = FPSuiComponent->CreateText(std::to_string((int)
				GEngine::Application::GetApp()->profile["FPS"]) + " FPS", font, { -1, .9f,1 },
				{ 1,1,1 }, { 1,1,1,1 });
		}

	}
	
	Ref<CharacterEntity> characterEntity;
	GEngine::Ref<AudioSource> source;

	
	inline virtual void OnEvent(GEngine::Event& e) override {

#if defined(GE_CONSOLE_APP) && !defined(GE_DIST)
		if (!Application::InputEnabled()) return;
#endif

		m_CameraController->OnEvent(e);

		if (e.GetEventType() == EventType::KeyReleased) {
			GEngine::KeyReleasedEvent& _e = (GEngine::KeyReleasedEvent&)e;
			if (_e.GetKeyCode() == GE_KEY_P) {
				if (source->IsPlaying()) {
					source->Pause();
				}
				else {
					source->Play();
				}
			}
			
			if (_e.GetKeyCode() == GE_KEY_L) {
				source->SetLoop(!source->IsLooping());
			}
		}

	}



	inline void OnBegin() override
	{
		camera = m_CameraController->GetCamera().get();
		GEngine::Application::GetApp()->SetTargetCamera(camera);
		GEngine::Application::GetApp()->SetTargetCameraController(m_CameraController.get());
		GEngine::Application::GetApp()->GetTargetCameraController()->SetCameraZoom(10.f);
		m_CameraController->SetPosition({ 0,6.5,0 });
		m_CameraController->SetRotation({ 0,0,0 });

		FPSuiComponent = GEngine::CreateGameObject<GEngine::UIComponent>();

		GEngine::Ref<GEngine::Entity> eFPS = GEngine::CreateGameObject<GEngine::Entity>();
		eFPS->m_tag = "UI Entity";
		AddEntity(eFPS);
        
        Ref<LightComponent> lc = GEngine::CreateGameObject<LightComponent>();
        eFPS->AddComponent(lc);
        long l1 = lc->AddCircleLight({3,1}, 50, {1,1}, {1,0,0,.6f});
        long l2 = lc->AddCircleLight({3,3}, 50, {1,1}, {1,1,1,.5f});
       
		eFPS->AddComponent(FPSuiComponent);
		GEngine::Ref<GEngine::Texture2D > buttonTexture = GEngine::Texture2D::Create("Content/Textures/back_button.png");
		float size = max((float)Application::GetWidth() / (float)Application::GetUIResolutionWidth(), (float)Application::GetHeight() / (float)Application::GetUIResolutionHeight());
		float buttonY = size * ((float)buttonTexture->GetHeight() / (float)Application::GetHeight()) * 6.25f;
		float buttonX = size * ((float)buttonTexture->GetWidth() / (float)Application::GetWidth()) * 6.25f;
		GEngine::Ref<GEngine::ButtonComponent> button = GEngine::CreateGameObject<GEngine::ButtonComponent>(
			glm::vec3(.85, .90, 0), 0.f, glm::vec2(buttonX, buttonY), glm::vec4(1, 1, 1, 1.f));
		eFPS->AddComponent(button);
		button->SetImageTexture(buttonTexture);
		float lastButtonX = buttonX;
		float lastButtonY = buttonY;
		buttonTexture = GEngine::Texture2D::Create("Content/Textures/videoLife_button_7.png");
		buttonY = size * ((float)buttonTexture->GetHeight() / (float)Application::GetHeight()) * 5.f;
		buttonX = size * ((float)buttonTexture->GetWidth() / (float)Application::GetWidth()) * 5.f;
		GEngine::Ref<GEngine::ButtonComponent> button2 = GEngine::CreateGameObject<GEngine::ButtonComponent>(
			glm::vec3(.8 - (lastButtonX / 2.f) - buttonX/2.f, .90, 0), 0.f, glm::vec2(buttonX, buttonY), glm::vec4(1, 1, 1, 1.f));
		eFPS->AddComponent(button2);
		button2->SetImageTexture(buttonTexture);

		button->SetOnEvent([](const GEngine::Event& e) {

			if (e.GetEventType() == EventType::MouseButtonReleased) {
				GEngine::SceneManager::SetCurrentScene("menuScene");
			}

			if (e.GetEventType() == EventType::TouchReleased) {
				GEngine::SceneManager::SetCurrentScene("menuScene");
			}

		});

		//FPSuiComponent->CreateText("Ad", font, { .84f, .94f, 1.f }, { .5 ,.5,1 }, { 0,0,0,1 });

        button2->SetOnEvent([](const GEngine::Event& e){
			if (e.GetEventType() == EventType::TouchReleased) {
				if (GEngine::AdManager::AdLoaded()) {
					GEngine::AdManager::ShowRewardAd([](int amt, std::string type) { GE_LOG_INFO("Reward user with {0}, {1}", amt, type); },
						[](int state) { GE_LOG_DEBUG("Loading AD: "); GEngine::AdManager::LoadRewardAd([]() {GE_LOG_INFO("Ad Loaded!"); }); });
				}
				else {
					GEngine::AdManager::LoadRewardAd([]() {GE_LOG_DEBUG("AD LOADED");
					GEngine::AdManager::ShowRewardAd([](int amt, std::string type) { GE_LOG_INFO("Reward user with {0}, {1}", amt, type); },
						[](int state) {  GE_LOG_DEBUG("Loading AD: "); GEngine::AdManager::LoadRewardAd([]() {GE_LOG_INFO("Ad Loaded!"); }); });
						});
				}
			}
        });
	
		
		AddEntity(GEngine::CreateGameObject<BackgroundEntity>());
		AddEntity(GEngine::CreateGameObject<GameManagerEntity>());
		characterEntity = GEngine::CreateGameObject<CharacterEntity>();
		AddEntity(characterEntity);


		/* test fire
		
		
		*/

		auto fireEnt = GEngine::CreateGameObject<FireEntity>();
		AddEntity(fireEnt);


		/* test fire


		*/



		/**
		 * CHUNK
		 */


       // AddEntity(GEngine::CreateGameObject<GroundEntity>());
		
		/**
		 *  Platforms
		 */

		//AddEntity(GEngine::CreateGameObject<BackgroundTiledEntity>(glm::vec2(0, 10), glm::vec2(34.f, 20)));


		/*
		std::vector <glm::vec4> pos = ProceduralPlatformGeneration::GenerateLevel(GEngine::Time::GetEpochTimeMS(), { -10,0 }, { 10, 20 }, { 1, 1 }, { 3,3 }, { .1f,.1f }, { characterEntity->m_characterComponent->jumpXDragClamp,characterEntity->m_characterComponent->jumpYDragClamp }, [this](float x, float y) { return characterEntity->m_characterComponent->CalculateJumpVelocity(x, y); }, .5f);

		for (const glm::vec4& p : pos) {
			AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(p.x, p.y), glm::vec2(p.z, p.w)));

		}
		*/


		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(10, 10), glm::vec2(5, 20)));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(-10, 10), glm::vec2(5, 20)));
		

		/*

		std::vector<glm::vec4> pos = ProceduralPlatformGeneration::GenerateLevel(GEngine::Time::GetEpochTimeSec(), { -5,0 }, { 5, 20 }, { 1,1 }, { 3,3 }, { -CharacterController::jumpXDragClamp, 0}, { CharacterController::jumpXDragClamp, CharacterController::jumpYDragClamp }, { 1,2 }, [](float x, float y) {return CharacterController::CalculateJumpVelocity(x, y); }, .5f);

		for (glm::vec4 p : pos) {
			AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(p.x, p.y), glm::vec2(p.z, p.w)));
		}
		

		*/
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(0, -5.f), glm::vec2(25, 10)) );
		
		
		
		// Castle Wall
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(3.5f, 16.7f), glm::vec2(4.f, 1.f), 5.f, "CastleTop01"));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(2.f, 9.3f), glm::vec2(1.f, 14.f),  5.f, "CastleWall01"));

		// Inside Castle
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(6.7f, 2.2f), glm::vec2(2.f, 1.f), 4.f, "Platform_Castle01"));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(3.3f, 4.4f), glm::vec2(2.f, 1.f), 4.f, "Platform_Castle02"));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(7.2f, 6.6f), glm::vec2(2.f, 1.f), 4.f, "Platform_Castle03"));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(3.3f, 10.f), glm::vec2(2.f, 1.f), 4.f, "Platform_Castle04"));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(7.2f, 13.f), glm::vec2(2.f, 1.f), 4.f, "Platform_Castle05"));

		/*
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(1.f, 11.f), glm::vec2(3.f, 2.f)));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(-2.5f, 14.f), glm::vec2(2.f, 2.f)));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(-7.f, 15.f), glm::vec2(1.f, 2.f)));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(-1.5f, 18.f), glm::vec2(3.f, 1.f)));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(3.f, 18.f), glm::vec2(3.f, 1.f)));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(2.5f, 21.5f), glm::vec2(1.f, .5f)));
		AddEntity(GEngine::CreateGameObject<PlatformEntity>(glm::vec2(1.f, 22.f), glm::vec2(2.f, 1.f)));
		*/


		/**
		 * END CHUNK
		 */
	
        



	}



	inline void OnEnd() override
	{

	}

	inline void OnImGuiRender() override {

#if defined(GE_CONSOLE_APP) && !defined(GE_DIST)

		if (!DebugLayer::showLog) return;
		ImGui::Begin("Debug Info", &DebugLayer::showLog);
		ImGui::Separator();
		ImGui::Text("Character: ");
		ImGui::Separator();
		ImGui::Text("Player Position:  (%f, %f, %f)", characterEntity->GetEntityPosition().x, characterEntity->GetEntityPosition().y, characterEntity->GetEntityPosition().z);
		ImGui::Text("Player Projected Velocity:  (%f, %f)", characterEntity->m_characterComponent->_realVel.x, characterEntity->m_characterComponent->_realVel.y);
		ImGui::Text("Player Velocity Velocity:  (%f, %f)", characterEntity->m_characterComponent->bodyComp->GetVelocity().x, characterEntity->m_characterComponent->bodyComp->GetVelocity().y);
		ImGui::Separator();
		ImGui::Text("Touch Info");
		ImGui::Separator();
		ImGui::Text("Touch Count: %d", characterEntity->m_characterComponent->m.size());
		if (characterEntity->m_characterComponent->m.size() == 1) {
			ImGui::Text("Touch id: %lld", characterEntity->m_characterComponent->m.begin()->first);
			ImGui::Text("Touch State: %d", characterEntity->m_characterComponent->m.begin()->second.GetState());
			ImGui::Text("Touch X: %f", characterEntity->m_characterComponent->m.begin()->second.GetX());
			ImGui::Text("Touch Y: %f", characterEntity->m_characterComponent->m.begin()->second.GetY());
			ImGui::Text("Touch Force: %f", characterEntity->m_characterComponent->m.begin()->second.GetForce());
			ImGui::Text("Touch Time: %lld", characterEntity->m_characterComponent->m.begin()->second.GetTime());
		}

		ImGui::Separator();

		ImGui::Separator();
		ImGui::Text("Camera");
		ImGui::Separator();
		ImGui::Text("Camera Position: (%f, %f, %f)", m_CameraController->GetPosition().x, m_CameraController->GetPosition().y, m_CameraController->GetPosition().z);
		ImGui::Text("Camera Rotation: (%f, %f, %f)", m_CameraController->GetRotation().x, m_CameraController->GetRotation().y, m_CameraController->GetRotation().z);
		ImGui::Text("Camera Zoom: %f", m_CameraController->GetFOV());
		ImGui::Separator();
		ImGui::Text("Mouse");
		ImGui::Text("Mouse Position: (%f, %f}", Input::GetMousePosition().first, Input::GetMousePosition().second);
		ImGui::Separator();

		if (ImGui::Button("ReloadGraphics")) {
			GEngine::Application::UnloadGraphics();
			GEngine::Application::GetApp()->QueueGraphicsApi(GEngine::FGraphicsApi::OPENGL);
			GEngine::Application::ReloadGraphics();
		}


		ImGui::End();


#endif


	}

	inline void OnLoad() override
	{
		SetupCamera();

		font = GEngine::Font::Create("Content/Fonts/arial.ttf", 120.f );
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
	std::string textId;
	int targetWidth = 1080, targetHeight = 1920;



	inline void SetupCamera() {
		m_CameraController = std::unique_ptr<GEngine::Orthographic_CameraController>(new GEngine::Orthographic_CameraController(
			(float)GEngine::Application::GetWidth()/ (float)GEngine::Application::GetHeight()));
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
