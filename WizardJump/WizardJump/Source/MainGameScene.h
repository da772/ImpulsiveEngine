#pragma once
#include <GEngine.h>
#include "DebugLayer.h"
#include "Game/GameManager.hpp"
#include "Character/CharacterEntity.h"
#include "Environment/BackgroundEntity.hpp"
#include "Environment/ColliderEntity.hpp"
#include "Environment/WallEntity.hpp"
#include "Environment/PlatformEntity.hpp"
#include "Environment/Objects/FireEntity.hpp"
#include "Character/CharacterController.hpp"
#include "Generation/ProceduralPlatformGeneration.hpp"
#include "Environment/Objects/LightGlowEntity.hpp"

#include "Environment/SpriteEntity.hpp"

#include "Environment/WindowEntity.hpp"


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
				GEngine::Application::GetApp()->profile["FPS"]) + " fps", font, { -1, .9f,1 },
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


		

		Ref<SpriteComponent> sp = GEngine::CreateGameObject<SpriteComponent>();
		eFPS->AddComponent(sp);


       // long l1 = lc->AddCircleLight({-3,3}, 50, {4,4}, { 175.f / 255.f,204.f / 255.f,225.f / 255.f,.8f });
       // long l2 = lc->AddCircleLight({3,3}, 50, {1,1}, {1,1,1,.5f});
       
		eFPS->AddComponent(FPSuiComponent);
		GEngine::Ref<GEngine::Texture2D > buttonTexture = GEngine::Texture2D::Create("Content/Textures/back_button.png");
		float sizeX = (float)Application::GetWidth() / (float)Application::GetUIResolutionWidth();
		float sizeY = (float)Application::GetHeight() / (float)Application::GetUIResolutionHeight();

		
		float buttonY = sizeY * ((float)buttonTexture->GetHeight() / (float)Application::GetHeight()) * 6.25f;
		float buttonX = sizeX * ((float)buttonTexture->GetWidth() / (float)Application::GetWidth()) * 6.25f;
		GEngine::Ref<GEngine::ButtonComponent> button = GEngine::CreateGameObject<GEngine::ButtonComponent>(
			glm::vec3(.85, .90, 0), 0.f, glm::vec2(buttonX, buttonY), glm::vec4(1, 1, 1, 1.f));
		eFPS->AddComponent(button);
		button->SetImageTexture(buttonTexture);
		float lastButtonX = buttonX;
		float lastButtonY = buttonY;
		buttonTexture = GEngine::Texture2D::Create("Content/Textures/videoLife_button_7.png");
		buttonY = sizeY * ((float)buttonTexture->GetHeight() / (float)Application::GetHeight()) * 5.f;
		buttonX = sizeX * ((float)buttonTexture->GetWidth() / (float)Application::GetWidth()) * 5.f;
		GEngine::Ref<GEngine::ButtonComponent> button2 = GEngine::CreateGameObject<GEngine::ButtonComponent>(
			glm::vec3(.8 - (lastButtonX / 2.f) - buttonX/2.f, .90, 0), 0.f, glm::vec2(buttonX, buttonY), glm::vec4(1, 1, 1, 1.f));
		eFPS->AddComponent(button2);
		button2->SetImageTexture(buttonTexture);

		buttonTexture = Texture2D::Create("Content/Textures/dialogFrame.png", TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_DisableMipMap);
		buttonY = sizeY * ((float)buttonTexture->GetHeight() / (float)Application::GetHeight())*1.79f;
		buttonX = sizeX * ((float)buttonTexture->GetWidth() / (float)Application::GetWidth())*1.79f;
		buttonX = (float)buttonTexture->GetWidth() / (float)GEMath::max(buttonTexture->GetWidth(), buttonTexture->GetHeight());
		buttonY = (float)buttonTexture->GetHeight() / (float)GEMath::max(buttonTexture->GetWidth(), buttonTexture->GetHeight());
		
		float ___scale = 1.125f;
		FPSuiComponent->CreateQuad({ 0,.5f,5 }, 0, { ___scale*(buttonX *Application::GetWidth()/(float)Application::GetWidth()), ___scale * (buttonY * (float)Application::GetHeight())/(float)Application::GetHeight(), 1 }, { 1,1,1,1 }, buttonTexture);

		___scale = .275f;
		buttonTexture = Texture2D::Create("Content/Textures/wiz10_face.png", TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_DisableMipMap);
		buttonX = (float)buttonTexture->GetWidth() / (float)GEMath::max(buttonTexture->GetWidth(), buttonTexture->GetHeight());
		buttonY = (float)buttonTexture->GetHeight() / (float)GEMath::max(buttonTexture->GetWidth(), buttonTexture->GetHeight());
		FPSuiComponent->CreateQuad({ -.735f,.5f,4 }, 0, { ___scale * (buttonX * Application::GetWidth() / (float)Application::GetWidth()), ___scale * (buttonY * (float)Application::GetHeight()) / (float)Application::GetHeight(), 1 }, { 1,1,1,1 }, buttonTexture);

		FPSuiComponent->CreateText("Wizard", font, { -.45f, .55f, 6.f }, { .5f, .5f, 2.f }, { 1,1,1,1 });
		FPSuiComponent->CreateText("This is a big mountain. I wonder how I got up here. Maybe I should search that tower for clues so I can get out of here!", font, { -.45f, .5f, 6.f }, { .35f, .35f, 1.4f }, { 1,1,1,1 });

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
			GE_CORE_DEBUG("TOUCH RELEASED!");
			if (e.GetEventType() == EventType::TouchReleased) {
				GE_CORE_DEBUG("AD REQUESTED!");
				if (GEngine::AdManager::AdLoaded()) {
					GE_CORE_DEBUG("AD LOADED!");
					GEngine::AdManager::ShowRewardAd([](int amt, std::string type) { GE_LOG_INFO("Reward user with {0}, {1}", amt, type); },
						[](int state) { GE_LOG_DEBUG("Loading AD: "); GEngine::AdManager::LoadRewardAd([]() {GE_LOG_INFO("Ad Loaded!"); }); });
				}
				else {
					GE_CORE_DEBUG("AD LOADING!");
					GEngine::AdManager::LoadRewardAd([]() {GE_LOG_DEBUG("AD LOADED");
					GEngine::AdManager::ShowRewardAd([](int amt, std::string type) { GE_LOG_INFO("Reward user with {0}, {1}", amt, type); },
						[](int state) {  GE_LOG_DEBUG("Loading AD: "); GEngine::AdManager::LoadRewardAd([]() {GE_LOG_INFO("Ad Loaded!"); }); });
						});
				}
			}
        });
	
		AddEntity(GEngine::CreateGameObject<GameManagerEntity>());
		characterEntity = GEngine::CreateGameObject<CharacterEntity>(glm::vec2(-10.f, -6.5f));
		AddEntity(characterEntity);

		Ref<BackgroundEntity> bg = CreateGameObject<BackgroundEntity>();
		AddEntity(bg);

		//Ref<AudioComponent> audioS = CreateGameObject<AudioComponent>("Content/Audio/countdown.ogg", true, true, true);
		//bg->AddComponent(audioS);

		AddEntity(CreateGameObject<ColliderEntity>(glm::vec3(0,-7.9, 3), glm::vec2(25, 1), 0.f,"ground"));
		AddEntity(CreateGameObject<ColliderEntity>(glm::vec3(-11.6f, -5, 3), glm::vec2(1.f, 25),.5f, "wall"));
		AddEntity(CreateGameObject<ColliderEntity>(glm::vec3(2.f, 7, 3), glm::vec2(1.f, 25),.5f, "wall2"));
		AddEntity(CreateGameObject<ColliderEntity>(glm::vec3(11.25f, 5, 3), glm::vec2(1.f, 25),.5f, "wall3"));
		AddEntity(CreateGameObject<ColliderEntity>(glm::vec3(6.5f, 7.0f, 3), glm::vec2(9.f, 1), .5f, "wall4"));


		AddEntity(CreateGameObject<ColliderEntity>(glm::vec3(10.25f, -4.5f, 3), glm::vec2(3.f, .25f), .0f, "ground"));
		AddEntity(CreateGameObject<PlatformEntity>(glm::vec2(9.f, -4.5f), glm::vec2(3.f, 1)));

		AddEntity(CreateGameObject<ColliderEntity>(glm::vec3(6.6f, -1.f, 3), glm::vec2(1.75f, .25f), .0f, "ground"));
		AddEntity(CreateGameObject<PlatformEntity>(glm::vec2(6.f, -1.f), glm::vec2(2.5f, 1)));

		AddEntity(CreateGameObject<ColliderEntity>(glm::vec3(10.f, 2.f, 3), glm::vec2(2.f, .25f), .0f, "ground"));
		AddEntity(CreateGameObject<PlatformEntity>(glm::vec2(9.3f, 2.f), glm::vec2(2.5f, 1)));

		AddEntity(CreateGameObject<ColliderEntity>(glm::vec3(4.5f, 4.f, 3), glm::vec2(4.f, .25f), .0f, "ground"));
		AddEntity(CreateGameObject<PlatformEntity>(glm::vec2(2.7f, 4.f), glm::vec2(7.f, 1)));
		

		bg->AddParalaxBackground("back", Texture2D::Create("Content/Textures/MountainBackground/background1.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Wrap_ClampToEdge), { 20,20 }, .7, -5, { 0, -5});
		bg->AddParalaxBackground("backL", Texture2D::Create("Content/Textures/MountainBackground/background1.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Wrap_ClampToEdge), { 20,20 }, .7, -5, { -10, -5 });
		bg->AddParalaxBackground("backR", Texture2D::Create("Content/Textures/MountainBackground/background1.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Wrap_ClampToEdge), { 20,20 }, .7, -5, { 10, -5 });
		bg->AddParalaxBackground("middle", Texture2D::Create("Content/Textures/MountainBackground/background2.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Wrap_ClampToEdge), { 20,20 }, .5f, -4, { 0,-5 });
		bg->AddParalaxBackground("middleL", Texture2D::Create("Content/Textures/MountainBackground/background2.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Wrap_ClampToEdge), { 20,20 }, .5f, -4, { -10,-5 });
		bg->AddParalaxBackground("middleR", Texture2D::Create("Content/Textures/MountainBackground/background2.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Wrap_ClampToEdge), { 20,20 }, .5f, -4, { 10,-5 });
		bg->AddParalaxBackground("front", Texture2D::Create("Content/Textures/MountainBackground/background3.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Wrap_ClampToEdge), { 20,20 }, .3f, -3, { 0,-5 });
		bg->AddParalaxBackground("frontL", Texture2D::Create("Content/Textures/MountainBackground/background3.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Wrap_ClampToEdge), { 20,20 }, .3f, -3, { -10,-5 });
		bg->AddParalaxBackground("frontR", Texture2D::Create("Content/Textures/MountainBackground/background3.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Wrap_ClampToEdge), { 20,20 }, .3f, -3, { 10,-5 });

		Ref<LightComponent> lc = CreateGameObject<LightComponent>();
		bg->AddComponent(lc);
		lc->AddQuadLight({ -98.f,0.f }, 0.f, { 200.f,200.f }, { 0.f,0.f,0.f,300.f });
		lc->AddQuadLight({  111.f,0.f }, 0.f, { 200.f,200.f }, { 0.f,0.f,0.f,300.f });
		lc->AddQuadLight({ 0.f,107.f }, 0.f, { 200.f,200.f }, { 0.f,0.f,0.f,300.f });
		lc->AddQuadLight({ 0.f,-108.f }, 0.f, { 200.f,200.f }, { 0.f,0.f,0.f,300.f });
		
		AddEntity(CreateGameObject<FireEntity>(glm::vec3(9.5f,-1,6)));
		AddEntity(CreateGameObject<FireEntity>(glm::vec3(9.5f, 5., 6)));
		AddEntity(CreateGameObject<FireEntity>(glm::vec3(4.f, 5., 6)));

		AddEntity(CreateGameObject<LightGlowEntity>(glm::vec3(10.23f,-6.7f,0 ),glm::vec2(1,1), glm::vec2(.25f,.25f), glm::vec4(0,1,0,.5f)));


		Ref<SpriteEntity> windowLights = CreateGameObject<SpriteEntity>(glm::vec3(0, 0, 0), glm::vec2(1, 1), 0.0f, "WindowLighting");
		AddEntity(windowLights);
		Ref<LightComponent> windowL = CreateGameObject<LightComponent>();
		windowLights->AddComponent(windowL);
		windowL->AddQuadLight({ 4.9f,-5.4f}, 0, { 5.125f, 5.031f }, { 1.f,1.f,1.f,.7f }, Texture2D::Create("Content/Textures/windowLight01.png"));
		windowL->AddQuadLight({ 4.9f,1.3f }, 0, { 5.125f, 5.031f }, { 1.f,1.f,1.f,.7f }, Texture2D::Create("Content/Textures/windowLight01.png"));

		

		Ref<SpriteEntity> spriteEntity = CreateGameObject<SpriteEntity>(glm::vec3(0, 0, 0), glm::vec2(1, 1), 0.0f, "Level01_Background");
		AddEntity(spriteEntity);
		Ref<SpriteComponent> comp = spriteEntity->GetSpriteComponent();
		long id=  comp->CreateQuad({ 0,0,2 }, 0, { 25,17,0 }, { 1,1,1,300.f }, Texture2D::Create("Content/Textures/level_01.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Wrap_ClampToEdge) );
		comp->CreateQuad({ 0,105,0 }, 0, { 200.f, 200.f ,1.f}, {104.f/255.f,181.f/255.f,223.f/255.f,1});
		comp->CreateQuad({ 0,-110,0 }, 0, { 200.f, 200.f ,1.f }, { 119.f / 255.f,112.f / 255.f,111.f / 255.f,1 });
		comp->CreateQuad({ 6.7f,-13.8f,3 }, 0, { 10.21f,11.78f,0 }, { .4f,.4f,.4f,1.f}, Texture2D::Create("Content/Textures/towerUnder.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Wrap_ClampToEdge));

		comp->CreateQuad({ -6.95f,-13.8f,3 }, 0, { 9.25f,11.78f,0 }, { .4f,.4f,.4f,1.f }, Texture2D::Create("Content/Textures/towerUnder.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Wrap_ClampToEdge));
		comp->CreateQuad({ -15.7f,-13.8f,3 }, 0, { 9.25f,11.78f,0 }, { .4f,.4f,.4f,1.f }, Texture2D::Create("Content/Textures/towerUnder.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Wrap_ClampToEdge));
		comp->CreateQuad({ -15.7f,-2.8f,3 }, 0, { 9.25f,11.78f,0 }, { .4f,.4f,.4f,1.f }, Texture2D::Create("Content/Textures/towerUnder.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Wrap_ClampToEdge));
		comp->CreateQuad({ -15.7f,8.8f,3 }, 0, { 9.25f,11.78f,0 }, { .4f,.4f,.4f,1.f }, Texture2D::Create("Content/Textures/towerUnder.png", TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Wrap_ClampToEdge));



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

		font = GEngine::Font::Create("Content/Fonts/Wizard.ttf", 120.f );
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
