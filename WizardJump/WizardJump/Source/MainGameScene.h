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
#include "Environment/FogEntity.h"

#include "Environment/CloudGenerator.h"

#include "UI/DialogFrame.hpp"

#include "Environment/SpriteEntity.hpp"

#include "Environment/WindowEntity.hpp"


#include "Lighting/LightComponent.h"


class MainGameScene : public GEngine::Scene {


public:

	inline MainGameScene(const char* name, GEngine::Camera* cam) : Scene(name, cam) {


	};

	inline virtual ~MainGameScene() {
	
		GE_LOG_DEBUG("DESTROYING MAIN GAME SCENE");
	};

	inline void OnUpdate(GEngine::Timestep timestep) override
	{
		long long time = GEngine::Time::GetEpochTimeMS();
		m_CameraController->OnUpdate(timestep);
		Vector3f pos;
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
	
	
	

	
	inline virtual void OnEvent(GEngine::Event& e) override {

#if defined(GE_CONSOLE_APP) && !defined(GE_DIST)
		if (!Application::InputEnabled()) return;
#endif

		m_CameraController->OnEvent(e);

		if (e.GetEventType() == EventType::KeyReleased) {
			GEngine::KeyReleasedEvent& _e = (GEngine::KeyReleasedEvent&)e;
			if (_e.GetKeyCode() == GE_KEY_P) {
				
				//AddEntity(CreateGameObject<DialogFrame>(Vector3f(0, .5f, 5), 15.f, "Wizard", "Content/Textures/wiz10_face.png", "This is a big mountain. I wonder how I got up here. Maybe I should search that tower for clues so I can get out of here! aisdjiasjd aisdji asid jasid jaisdj aisdj ai aisdjiasjd"));
				
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
			Vector3f(.85, .90, 1), 0.f, Vector2f(buttonX, buttonY), Vector4f(1, 1, 1, 1.f));
		eFPS->AddComponent(button);
		button->SetImageTexture(buttonTexture);
		float lastButtonX = buttonX;
		float lastButtonY = buttonY;
		buttonTexture = GEngine::Texture2D::Create("Content/Textures/videoLife_button_7.png");
		buttonY = sizeY * ((float)buttonTexture->GetHeight() / (float)Application::GetHeight()) * 5.f;
		buttonX = sizeX * ((float)buttonTexture->GetWidth() / (float)Application::GetWidth()) * 5.f;
		GEngine::Ref<GEngine::ButtonComponent> button2 = GEngine::CreateGameObject<GEngine::ButtonComponent>(
			Vector3f(.8 - (lastButtonX / 2.f) - buttonX/2.f, .90, 1), 0.f, Vector2f(buttonX, buttonY), Vector4f(1, 1, 1, 1.f));
		eFPS->AddComponent(button2);
		button2->SetImageTexture(buttonTexture);

		fog = CreateGameObject<FogEntity>();
		AddEntity(fog);


		


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
				GE_CORE_DEBUG("TOUCH RELEASED!");
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
		characterEntity = GEngine::CreateGameObject<CharacterEntity>(Vector2f(-10.f, -6.5f));
		AddEntity(characterEntity);

		Ref<BackgroundEntity> bg = CreateGameObject<BackgroundEntity>();
		AddEntity(bg);

		//Ref<AudioComponent> audioS = CreateGameObject<AudioComponent>("Content/Audio/countdown.ogg", true, true, true);
		//bg->AddComponent(audioS);

		AddEntity(CreateGameObject<ColliderEntity>(Vector3f(0,-7.9, 3), Vector2f(25, 1), 0.f,"ground"));
		AddEntity(CreateGameObject<ColliderEntity>(Vector3f(-11.6f, -5, 3), Vector2f(1.f, 25),.5f, "wall"));
		AddEntity(CreateGameObject<ColliderEntity>(Vector3f(2.f, 7, 3), Vector2f(1.f, 25),.5f, "wall2"));
		AddEntity(CreateGameObject<ColliderEntity>(Vector3f(11.25f, 5, 3), Vector2f(1.f, 25),.5f, "wall3"));
		AddEntity(CreateGameObject<ColliderEntity>(Vector3f(6.5f, 7.0f, 3), Vector2f(9.f, 1), .5f, "wall4"));


		AddEntity(CreateGameObject<ColliderEntity>(Vector3f(10.25f, -4.5f, 3), Vector2f(3.f, .25f), .0f, "ground"));
		AddEntity(CreateGameObject<PlatformEntity>(Vector2f(9.f, -4.5f), Vector2f(3.f, 1)));

		AddEntity(CreateGameObject<ColliderEntity>(Vector3f(6.6f, -1.f, 3), Vector2f(1.75f, .25f), .0f, "ground"));
		AddEntity(CreateGameObject<PlatformEntity>(Vector2f(6.f, -1.f), Vector2f(2.5f, 1)));

		AddEntity(CreateGameObject<ColliderEntity>(Vector3f(10.f, 2.f, 3), Vector2f(2.f, .25f), .0f, "ground"));
		AddEntity(CreateGameObject<PlatformEntity>(Vector2f(9.3f, 2.f), Vector2f(2.5f, 1)));

		AddEntity(CreateGameObject<ColliderEntity>(Vector3f(4.5f, 4.f, 3), Vector2f(4.f, .25f), .0f, "ground"));
		AddEntity(CreateGameObject<PlatformEntity>(Vector2f(2.7f, 4.f), Vector2f(7.f, 1)));
		
		
		bg->AddParalaxBackground("back", Texture2D::Create("Content/Textures/MountainBackground/background1.png", TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest| TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Wrap_ClampToEdge), { 16,8 }, .7, -14, { 0, 0.f});
		bg->AddParalaxBackground("backL", Texture2D::Create("Content/Textures/MountainBackground/background1.png", TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Wrap_ClampToEdge), { 16,8 }, .7, -14, { -8, 0.f });
		bg->AddParalaxBackground("backR", Texture2D::Create("Content/Textures/MountainBackground/background1.png", TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Wrap_ClampToEdge), { 16,8 }, .7, -14, { 8, 0.f });
		bg->AddParalaxBackground("middle", Texture2D::Create("Content/Textures/MountainBackground/background2.png", TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Wrap_ClampToEdge), { 16,8 }, .5f, -13, { 0,-5 });
		bg->AddParalaxBackground("middleL", Texture2D::Create("Content/Textures/MountainBackground/background2.png", TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Wrap_ClampToEdge), { 16,8 }, .5f, -13, { -8,-5 });
		bg->AddParalaxBackground("middleR", Texture2D::Create("Content/Textures/MountainBackground/background2.png", TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Wrap_ClampToEdge), { 16,8 }, .5f, -13, { 8,-5 });
		bg->AddParalaxBackground("front", Texture2D::Create("Content/Textures/MountainBackground/background3.png", TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Wrap_ClampToEdge), { 16,8 }, .3f, -12, { 0,-6 });
		bg->AddParalaxBackground("frontL", Texture2D::Create("Content/Textures/MountainBackground/background3.png", TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Wrap_ClampToEdge), { 16,8 }, .3f, -12, { -8,-6 });
		bg->AddParalaxBackground("frontR", Texture2D::Create("Content/Textures/MountainBackground/background3.png", TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Wrap_ClampToEdge), { 16,8 }, .3f, -12, { 8,-6 });

		
		Ref<LightComponent> lc = CreateGameObject<LightComponent>();
		bg->AddComponent(lc);
		lc->AddQuadLight({ -98.f,0.f }, 0.f, { 200.f,200.f }, { 0.f,0.f,0.f,300.f });
		lc->AddQuadLight({  111.f,0.f }, 0.f, { 200.f,200.f }, { 0.f,0.f,0.f,300.f });
		lc->AddQuadLight({ 0.f,107.f }, 0.f, { 200.f,200.f }, { 0.f,0.f,0.f,300.f });
		lc->AddQuadLight({ 0.f,-107.95f }, 0.f, { 200.f,200.f }, { 0.f,0.f,0.f,300.f });
		

		AddEntity(CreateGameObject<FireEntity>(Vector3f(9.5f,-1,6)));
		AddEntity(CreateGameObject<FireEntity>(Vector3f(9.5f, 5., 6)));
		AddEntity(CreateGameObject<FireEntity>(Vector3f(4.f, 5., 6)));

		AddEntity(CreateGameObject<LightGlowEntity>(Vector3f(10.23f,-6.7f,0 ),Vector2f(1,1), Vector2f(.25f,.25f), Vector4f(0,1,0,.5f)));

		
		Ref<SpriteEntity> windowLights = CreateGameObject<SpriteEntity>(Vector3f(0, 0, 0), Vector2f(1, 1), 0.0f, "WindowLighting");
		AddEntity(windowLights);
		Ref<LightComponent> windowL = CreateGameObject<LightComponent>();
		windowLights->AddComponent(windowL);
		windowL->AddQuadLight({ 4.9f,-5.4f}, 0, { 5.125f, 5.031f }, { 1.f,1.f,1.f,.7f }, Texture2D::Create("Content/Textures/windowLight01.png"));
		windowL->AddQuadLight({ 4.9f,1.3f }, 0, { 5.125f, 5.031f }, { 1.f,1.f,1.f,.7f }, Texture2D::Create("Content/Textures/windowLight01.png"));
		
		

		Ref<SpriteEntity> spriteEntity = CreateGameObject<SpriteEntity>(Vector3f(0, 0, 0), Vector2f(1, 1), 0.0f, "Level01_Background");
		AddEntity(spriteEntity);
		Ref<SpriteComponent> comp = spriteEntity->GetSpriteComponent();
		long id=  comp->CreateQuad({ 0,0,2 }, 0, { 25,17,0 }, { 1,1,1,300.f }, Texture2D::Create("Content/Textures/level_01.png", TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest | TEXTUREFLAGS_DisableMipMap | TEXTUREFLAGS_Wrap_ClampToEdge) );
		
		comp->CreateQuad({ 6.7f,-13.8f,3 }, 0, { 10.21f,11.78f,0 }, { .4f,.4f,.4f,1.f}, Texture2D::Create("Content/Textures/towerUnder.png", TEXTUREFLAGS_Wrap_ClampToEdge));

		comp->CreateQuad({ -6.95f,-13.8f,3 }, 0, { 9.25f,11.78f,0 }, { .4f,.4f,.4f,1.f }, Texture2D::Create("Content/Textures/towerUnder.png", TEXTUREFLAGS_Wrap_ClampToEdge));
		comp->CreateQuad({ -15.7f,-13.8f,3 }, 0, { 9.25f,11.78f,0 }, { .4f,.4f,.4f,1.f }, Texture2D::Create("Content/Textures/towerUnder.png", TEXTUREFLAGS_Wrap_ClampToEdge));
		comp->CreateQuad({ -15.7f,-2.8f,3 }, 0, { 9.25f,11.78f,0 }, { .4f,.4f,.4f,1.f }, Texture2D::Create("Content/Textures/towerUnder.png", TEXTUREFLAGS_Wrap_ClampToEdge));
		comp->CreateQuad({ -15.7f,8.8f,3 }, 0, { 9.25f,11.78f,0 }, { .4f,.4f,.4f,1.f }, Texture2D::Create("Content/Textures/towerUnder.png", TEXTUREFLAGS_Wrap_ClampToEdge));


		/**
		 * END CHUNK
		 */
		
		/* TUTORIAL BEGIN */
		


		AddEntity(CreateGameObject<CloudGenerator>(Vector3f( 20,0,10 ), Vector3f( -20,0,10 ), Vector2f(-1.f,1.f), 1.f, 10));

        tutorialCover = FPSuiComponent->CreateQuad({ -.5f,0,0 }, 0, { 0.f,0,1 }, { .25f,.25f,.25f,0 });

		characterEntity->m_characterComponent->bEnableInput = false;
		characterEntity->m_characterComponent->bEnableJump = false;
		Ref<DialogFrame> dialog = CreateGameObject<DialogFrame>(Vector3f(0, .5f, 5), 15.f, "Wizard", "Content/Textures/wiz10_face.png", "This is a big mountain. I wonder how I got up here. Maybe I should search that tower for clues so I can get out of here! \n(Tap here to continue...)", true, false);//aisdjiasjd aisdji asid jasid jaisdj aisdj ai aisdjiasjd->"));
	
		
		
		dialog->SetOnDialogComplete([this]() {
			
			Ref<DialogFrame> dialog = CreateGameObject<DialogFrame>(Vector3f(0, .5f, 5), 15.f, "Wizard", "Content/Textures/wiz10_face.png", "Press and hold on the right side of the screen to move to the right!", false, false);//aisdjiasjd aisdji asid jasid jaisdj aisdj ai aisdjiasjd->"));
			dialog->SetStickyFrame(true);
			AddEntity(dialog);
			Ref<SpriteAnimationComponent> spriteAnim = CreateGameObject<SpriteAnimationComponent>();
			dialog->AddComponent(spriteAnim);
			spriteAnim->SetFrameAnimation(30, 15, false, [this](int frame) {
				GE_LOG_DEBUG("FRAME ALPHA: {0}", (float)frame* (.9f / 15.f));
				FPSuiComponent->SetColor(tutorialCover, { .0f,.0f,.0f,(float)frame*(.9f/15.f) });
				FPSuiComponent->SetScale(tutorialCover, { (float)frame*(1.f/15.f),(float)frame*(2.f / 15.f),.0f});
				});
			spriteAnim->Start();
			
			characterEntity->m_characterComponent->bEnableInput = true;
			
			characterEntity->m_characterComponent->SetInputFilterFunction([this, dialog](const GEngine::FTouchInfo& touch) {
				// ensure we are moving right!
				if ((touch.state == 0 || touch.state == 1) && touch.x <= (float)Application::GetWidth() / 2.f) {
					return false;
				}
				else {
					if (characterEntity->m_characterComponent->bodyComp->GetVelocity().x > 2.f || characterEntity->GetEntityPosition().x > -8.f) {
						dialog->Destroy();
						characterEntity->m_characterComponent->bodyComp->SetVelocityX(0);
						characterEntity->m_characterComponent->bEnableInput = false;
						Ref<DialogFrame> _dialog = CreateGameObject<DialogFrame>(Vector3f(0, .5f, 5), 15.f, "Wizard", "Content/Textures/wiz10_face.png", "Press and hold on the left side of the screen to move to the left!", false, false);//aisdjiasjd aisdji asid jasid jaisdj aisdj ai aisdjiasjd->"));
						_dialog->SetStickyFrame(true);
						FPSuiComponent->SetColor(tutorialCover, { .0f,.0f,.0f, 0.f });
						FPSuiComponent->SetPosition(tutorialCover, { .5f, 0 });
						Ref<SpriteAnimationComponent> spriteAnim = CreateGameObject<SpriteAnimationComponent>();
						spriteAnim->SetFrameAnimation(30, 15, false, [this](int frame) {
							FPSuiComponent->SetColor(tutorialCover, { .0f,.0f,.0f,(float)frame * (.9f / 15.f) });
							FPSuiComponent->SetScale(tutorialCover, { (float)frame * (1.f / 15.f),(float)frame * (2.f / 15.f),.0f });
							});
						AddEntity(_dialog);
						_dialog->AddComponent(spriteAnim);
						spriteAnim->Start();
						
						characterEntity->m_characterComponent->SetInputFilterFunction([this, _dialog](const GEngine::FTouchInfo& touch) {
							// ensure we are moving left!
							if ( (touch.state == 0 || touch.state == 1) && touch.x >= (float)Application::GetWidth() / 2.f) {
								return false;
							}
							else {
								if (characterEntity->m_characterComponent->bodyComp->GetVelocity().x < -2.f || characterEntity->GetEntityPosition().x < -10.0f) {
									_dialog->Destroy();
									characterEntity->m_characterComponent->bodyComp->SetVelocityX(0);
									characterEntity->m_characterComponent->SetInputFilterFunction(nullptr);
									characterEntity->m_characterComponent->bEnableInput = false;
									Ref<DialogFrame> __dialog = CreateGameObject<DialogFrame>(Vector3f(0, .5f, 5), 15.f, "Wizard", "Content/Textures/wiz10_face.png", "I should keep walking to the right. \n(Tap here to continue...)", false, true);//aisdjiasjd aisdji asid jasid jaisdj aisdj ai aisdjiasjd->"));
									AddEntity(__dialog);
                                    FPSuiComponent->Remove(tutorialCover);
									__dialog->SetOnDialogComplete([this]() {
										characterEntity->m_characterComponent->bEnableInput = true;
										// Enable jump during jump tutorial
										characterEntity->m_characterComponent->bEnableJump = true;
										});
									
								}

							}


							return true;
							});
						characterEntity->m_characterComponent->bEnableInput = true;
					}
						
				}

				return true;
			});
				
			});
		AddEntity(dialog);

		
		/* TUTORIAL END */


	}

    
    ShapeID tutorialCover;


	inline void OnEnd() override
	{
		fog = nullptr;
		FPSuiComponent = nullptr;
	}


	Ref<FogEntity> fog;
	Ref<CharacterEntity> characterEntity;

	inline void OnImGuiRender() override {

#if defined(GE_CONSOLE_APP) && !defined(GE_DIST)

		if (!DebugLayer::showLog) return;

		ImGui::Begin("FogDebug", &DebugLayer::showLog);
		
		float lacunarity = fog->m_lacunarity, frequency = fog->m_frequency, gain = fog->m_gain, tickSpeed = fog->m_speed, speed = fog->m_moveSpeed;
		int octaves = fog->m_octaves, seed = fog->m_seed;


		ImGui::DragInt("Seed", &seed);
		ImGui::DragInt("Octaves", (int*)&octaves);
		ImGui::DragFloat("Lacunarity", &lacunarity, .25f);
		ImGui::DragFloat("Frequency", &frequency, .01f);
		ImGui::DragFloat("Gain", &gain, .25f);
		ImGui::DragFloat("Speed", &speed, .25f);
		ImGui::DragFloat("Tick Speed", &tickSpeed, 1.f);

		ImGui::End();

		if (fog->m_lacunarity != lacunarity) {
			fog->SetFractalLacunarity(lacunarity);
		}

		if (fog->m_octaves != octaves) {
			fog->SetOctaves(octaves);
		}

		if (fog->m_gain != gain) {
			fog->SetFractalGain(gain);
		}

		if (fog->m_frequency != frequency) {
			fog->SetFrequency(frequency);
		}

		if (fog->m_seed != seed) {
			fog->SetSeed(seed);
		}

		if (fog->m_moveSpeed != speed) {
			fog->SetSpeed(speed);
		}

		if (fog->m_speed != tickSpeed) {
			fog->SetTickSpeed(tickSpeed);
		}



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

		font = GEngine::Font::Create("Content/Fonts/Wizard.ttf", 120.f);
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
		m_CameraController->SetOnUpdateFn([this](GEngine::Timestep timeStep, Vector3f& m_Position, Vector3f& m_Rotation, Vector2f& m_LastTouchPos,
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

							Vector2f m_touchDir = { m_LastTouchPos.x - touches.begin()->second.x, m_LastTouchPos.y - touches.begin()->second.y };
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
