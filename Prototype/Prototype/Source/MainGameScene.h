#pragma once
#include <GEngine.h>
#include "DebugLayer.h"

class MainGameScene : public GEngine::Scene {


public:

	inline MainGameScene(const char* name, GEngine::Camera* cam) : Scene(name, cam) {


	};

	inline virtual ~MainGameScene() {};

	inline void OnUpdate(GEngine::Timestep timestep) override
	{
		long long time = GEngine::Time::GetEpochTimeMS();

		m_CameraController->OnUpdate(timestep);
		tCount += .1f*timestep;

		if (frameCount % 5 == 0) {
			//particles->Emit({ -0, 0, 1e6 }, 1);
			//particles2->Emit({ .0f, 0, 1e6 }, 1);
			frameCount = 1;
		}
		frameCount ++;

		float t = timestep;
		std::string retStr;

		
		/*
		if (GEngine::Input::IsKeyPressed(GE_KEY_RIGHT)) {
			mainEntity->SetEntityPosition(GEngine::Vector3(mainEntity->GetEntityPosition().x + 1 * timestep, mainEntity->GetEntityPosition().y, 0));
		}

		if (GEngine::Input::IsKeyPressed(GE_KEY_LEFT)) {
			mainEntity->SetEntityPosition(GEngine::Vector3(mainEntity->GetEntityPosition().x - 1 * timestep, mainEntity->GetEntityPosition().y, 0));
		}

		if (GEngine::Input::IsKeyPressed(GE_KEY_UP)) {
			mainEntity->SetEntityPosition(GEngine::Vector3(mainEntity->GetEntityPosition().x, mainEntity->GetEntityPosition().y + 1 * timestep, 0));
		}

		if (GEngine::Input::IsKeyPressed(GE_KEY_DOWN)) {
			mainEntity->SetEntityPosition(GEngine::Vector3(mainEntity->GetEntityPosition().x, mainEntity->GetEntityPosition().y - 1 * timestep, 0));
		}
		*/


		degree += 1 * timestep;
		if (degree >= 360)
			degree = 0;

		float xPos = radius * cos(degree);
		float yPos = radius * sin(degree);


		ent1->SetEntityPosition(glm::vec3(xPos+5, yPos, 1));
		xPos = radius+.2 * cos(-degree);
		yPos = radius+.2 * sin(-degree);
		ent2->SetEntityPosition(glm::vec3(xPos+4, yPos, 1));
		
		//mainEntity->SetEntityPosition(GEngine::Vector3(xPos, yPos, 0));

		if (FPSuiComponent) {

			if (textId != 0) {
				FPSuiComponent->RemoveText(textId);
				textId = 0;
			}

			textId = FPSuiComponent->CreateText(std::to_string((int)GEngine::Application::GetApp()->profile["FPS"]) + " FPS", font, { -1,0,1 }, { 0,0,1 }, { 1,1,1,1 });
		}

	}
	
	float degree = 0;
	float radius = .5f;

	long long lastTime = 0;
	int frameCount = 1;
	long textId = -1;
	

	inline virtual void OnEvent(GEngine::Event& e) override {
		m_CameraController->OnEvent(e);


		if (e.GetEventType() == GEngine::EventType::TouchPressed) {
			GEngine::TouchPressedEvent& event = (GEngine::TouchPressedEvent&)e;
			glm::vec3 position = GEngine::Utility::ScreenPosToWorldPos(glm::vec2(event.GetX(), event.GetY()) );
			//particles->Emit({ position.x, position.y, 1e5 }, 50);
		}

		if (e.GetEventType() == GEngine::EventType::MouseButtonPressed) {
			GEngine::MouseButtonEvent& event = (GEngine::MouseButtonEvent&)e;
			if (event.GetMouseButton() == GE_MOUSE_BUTTON_1) {
				glm::vec3 position = GEngine::Utility::ScreenPosToWorldPos(glm::vec2(GEngine::Input::GetMouseX(), GEngine::Input::GetMouseY()));
				//particles->Emit({ position.x, position.y, 1e5 }, 50);
				
			}
			if (event.GetMouseButton() == GE_MOUSE_BUTTON_2) {
				if (ids.size() > 0) {
					//long id = ids[ids.size() - 1];
					//ids.pop_back();
					//batcher->RemoveShape(id);
				}

			}

			if (event.GetMouseButton() == GE_MOUSE_BUTTON_3) {
				if (ids.size() > 0) {
					//long id = ids[ids.size() - 1];
					//batcher->EditShape(id, { 0,0,100 }, 0, { 1,1 }, { 0,1,0,1 }, spriteSheet1, 1);
					//batcher->RemoveShape(id);
				}
			}
			
			
		}

	}

	float tCount = 0;
	std::vector<long> ids;
	uint32_t scriptId = 0;

	inline void OnBegin() override
	{
		Setup();
		m_CameraController->SetCameraZoom(7.f);
		GEngine::AdManager::SetUserId("This Is My User ID!");
#ifdef GE_PLATFORM_ANDROID
		
		GEngine::AdManager::SetAdId("ca-app-pub-0400118858384122~7825957542");
		// Prototype Ad
		GEngine::AdManager::SetRewardAdId("ca-app-pub-0400118858384122/3886712532");
		// Google Test Id
		//GEngine::AdManager::SetRewardAdId("ca-app-pub-3940256099942544/5224354917");
#endif
#ifdef GE_PLATFORM_IOS
		GEngine::AdManager::SetAdId("ca-app-pub-0400118858384122~7797875681");
		GEngine::AdManager::SetRewardAdId("ca-app-pub-3940256099942544/5224354917");
		
#endif
		/*x
		GEngine::AdManager::LoadRewardAd([]() {GE_LOG_DEBUG("AD LOADED"); GEngine::AdManager::ShowRewardAd(); },
			[](int i, std::string s) { GE_CORE_DEBUG("AD WATCHED {0} : {1}", i, s);
			GEngine::AdManager::LoadRewardAd([]() {GE_LOG_DEBUG("AD LOADED"); GEngine::AdManager::ShowRewardAd(); },
			[](int i, std::string s) { GE_CORE_DEBUG("AD WATCHED {0} : {1}", i, s); });
		});
		*/
			

		/*
		GEngine::Ref<GEngine::FileData> fileDat = GEngine::FileSystem::FileDataFromPath(GEngine::Utility::FilePathFromRoot("Content/scripts/test.js"));

		GEngine::Ref<GEngine::ScriptObject> obj = GEngine::ScriptManager::GetApi()->CreateObject((char*)fileDat->GetDataAsString());
		GEngine::Ref<GEngine::ScriptTestClass> c = GEngine::Ref<GEngine::ScriptTestClass>(obj->asClass<GEngine::ScriptTestClass>());

		GE_CORE_DEBUG("ScriptTestClass: id:{0}", c->m_id);

		GEngine::Ref<GEngine::ScriptObject> _newObj = obj->CallMethod<float>("update", 2.2);
		GE_CORE_DEBUG("FUNCTION UPDATE: {0}", _newObj->asFloat());

		GEngine::Ref<GEngine::ScriptObject> _funcObj = obj->GetProp("func");
		GEngine::Ref<GEngine::ScriptObject> _funcRet = _funcObj->CallSelf<int>(22);
		GE_CORE_DEBUG("Function Returned: {0}", _funcRet->asFloat());

		GEngine::Ref<GEngine::ScriptObject> _linkedObj = obj->GetProp("linked");
		GEngine::Ref<GEngine::ScriptTestClass> linkedTestClass = GEngine::Ref<GEngine::ScriptTestClass>(_linkedObj->asClass<GEngine::ScriptTestClass>());
		GE_CORE_DEBUG("Linked Object: {0}", linkedTestClass->getId());

		*/

		/*
#ifndef GE_RELEASE
		GE_LOG_INFO("CREATING SERVER");
		GEngine::Networking::CreateServer(30001);
#else
		GE_LOG_INFO("JOINING SERVER");
		GEngine::Networking::ConnectToServer("127.0.0.1", 30001, 1000, 5000, [](bool b) {GE_LOG_INFO(b ? "Connected!" : "Failed to Connect"); });
#endif

	*/


	}



	inline void OnEnd() override
	{
		GEngine::Networking::DisconnectFromServer();
	}

	inline void OnImGuiRender() override {
		if (DebugLayer::showLog) {
		ImGui::Begin("Unit Stats");
		{


			for (auto k : GEngine::Application::GetApp()->profile) {
				ImGui::Text("%s %.3g ms", k.first.c_str(), k.second);
			}

			glm::vec3 position = { GEngine::Input::GetMouseX(),  GEngine::Input::GetMouseY(), 0 };
			int width, height;
			GEngine::Application::GetApp()->GetWindow()->GetFrameBufferSize(&width, &height);
			width = GEngine::Application::GetApp()->GetWindow()->GetWidth();
			height = GEngine::Application::GetApp()->GetWindow()->GetHeight();
			position = glm::unProject(position, m_CameraController->GetCamera()->GetViewMatrix(),
				m_CameraController->GetCamera()->GetProjectionMatrix(), glm::vec4(0, height, width, -height));
			ImGui::Text("Mouse X: %f, Y: %f", position.x, position.y);
			ImGui::Text("Particles: %d", particles->ParticleCount());
		}
		ImGui::End();

		ImGui::Begin("Networking"); {
			if (ImGui::Button("Host")) {
				GEngine::Networking::CreateServer();
			}

			if (ImGui::Button("Join")) {
				GEngine::Networking::ConnectToServer();
			}

			if (ImGui::Button("Disconnect")) {
				GEngine::Networking::DisconnectFromServer(3000, []() {GE_LOG_DEBUG("Disconnected from server!"); });
			}

			ImGui::Separator();
			ImGui::InputText("Packet data: ", &ch[0], 255);
			if (ImGui::Button("Send Packet")) {
				GEngine::Networking::SendPacket(ch);
			}
		}
		ImGui::End();
		}
	}

	inline void OnLoad() override
	{
		SetupCamera();
		spriteSheet1 = GEngine::SubTexture2D::CreateFromCoords(
			GEngine::Texture2D::Create("Content/Textures/guard1.png"), { 0,0 }, { 16,16 });

		spriteSheet2 = GEngine::SubTexture2D::CreateFromCoords(
			GEngine::Texture2D::Create("Content/Textures/snowLand.png",
				TEXTUREFLAGS_Wrap_ClampToEdge | TEXTUREFLAGS_DisableMipMap), { 0,0 }, { 128,128 }, { 5,5 });

		checkerBoardTexture = GEngine::Texture2D::Create("Content/Textures/Checkerboard.png", TEXTUREFLAGS_Wrap_Repeat);

		

		font = GEngine::Font::Create("Content/Fonts/Vera.ttf", 120);
		font->LoadCharactersEN();

	}


	inline void OnUnload() override
	{
		//m_CameraController = nullptr;
		spriteSheet1 = nullptr;
		spriteSheet2 = nullptr;
		checkerBoardTexture = nullptr;
		font = nullptr;
		batchComponent = nullptr;
		uiComponent = nullptr;
		if (mainEntity != nullptr)
			mainEntity->Destroy();
		mainEntity = nullptr;
	}

private:

	GEngine::Ref<GEngine::Orthographic_CameraController> m_CameraController;
	GEngine::Ref<GEngine::SubTexture2D> spriteSheet1;
	GEngine::Ref<GEngine::SubTexture2D> spriteSheet2;
	GEngine::Ref<GEngine::Texture2D> checkerBoardTexture;
	GEngine::Ref<GEngine::SpriteComponent> batchComponent;
	GEngine::Ref<GEngine::UIComponent> uiComponent;
	GEngine::Ref<GEngine::UIComponent> FPSuiComponent;
	GEngine::Ref<GEngine::Entity> mainEntity;
	GEngine::Ref<GEngine::Entity> fpsEntity;
	GEngine::Ref<GEngine::Font> font;
	GEngine::Ref<GEngine::ParticleSystem2DComponent> particles;
	GEngine::Ref<GEngine::ParticleSystem2DComponent> particles2;
	bool bImGui = true;
	char ch[255] = "Hi";

	inline void Setup() {
		camera = m_CameraController->GetCamera().get();
		GEngine::Application::GetApp()->SetTargetCamera(camera);
		GEngine::Application::GetApp()->SetTargetCameraController(m_CameraController.get());
		//m_CameraController->SetPosition({ 0,0,0 });
		m_CameraController->SetRotation({ 0,0,0 });

		mainEntity = GEngine::CreateGameObject<GEngine::Entity>();
		fpsEntity = GEngine::CreateGameObject<GEngine::Entity>();
		batchComponent = GEngine::CreateGameObject<GEngine::SpriteComponent>();
		FPSuiComponent = GEngine::CreateGameObject<GEngine::UIComponent>();
		uiComponent = GEngine::CreateGameObject<GEngine::UIComponent>();
		GEngine::Ref<GEngine::Entity> e = GEngine::CreateGameObject<GEngine::Entity>();
		e->AddComponent(GEngine::CreateGameObject<GEngine::ScriptComponent>("Content/Scripts/TestComponent.js"));
		mainEntity->AddComponent(batchComponent);
		mainEntity->AddComponent(uiComponent);
		fpsEntity->AddComponent(FPSuiComponent);
		

		ent1 = GEngine::CreateGameObject<GEngine::Entity>(glm::vec3(5.0,0,0));
		//GEngine::Ref<GEngine::SpriteComponent> spr1 = GEngine::CreateGameObject<GEngine::SpriteComponent>();

		//ent1->AddComponent(spr1);
		//spr1->CreateQuad(glm::vec3(0, 0, 0), 0, glm::vec3(1, 1, 1), glm::vec4(0, 0, 1, .75f));
		GEngine::Ref<GEngine::QuadColliderComponent> quadCollision = GEngine::CreateGameObject<GEngine::QuadColliderComponent>(true);
		
		ent2 = GEngine::CreateGameObject<GEngine::Entity>(glm::vec3(4.5, 0, 0));
		GEngine::Ref<GEngine::SpriteComponent> spr2 = GEngine::CreateGameObject<GEngine::SpriteComponent>();
		ent2->AddComponent(spr2);
		long n1 = spr2->CreateQuad(glm::vec3(0, 0, 0), 0, glm::vec3(1, 1, 1), glm::vec4(1, 0, 0, .75f));

		GEngine::Ref<GEngine::QuadColliderComponent> quadCollision2 = GEngine::CreateGameObject<GEngine::QuadColliderComponent>(true);
		quadCollision2->SetOnCollideFunction([n1, spr2](GEngine::Ref<GEngine::QuadColliderComponent> comp) {
			//GE_CORE_DEBUG("Colliding with Object 2");
			spr2->SetQuadColor(n1, glm::vec4(0, 0, 1, 1));
			});
		quadCollision2->SetEndCollideFunction([n1, spr2](GEngine::Ref<GEngine::QuadColliderComponent> comp) {
			//GE_CORE_DEBUG("Stopped Colliding with Object 2");
			spr2->SetQuadColor(n1, glm::vec4(1, 0, 0, 1));
			});



		//ent1->AddComponent(quadCollision);
		ent2->AddComponent(quadCollision2);

		mainEntity->SetShouldUpdate(true);
		batchComponent->CreateQuad({ 0,0,-1e5f }, 0, { 1000,1000,1 }, glm::vec4(.15f, .15f, .15f, 1.f), checkerBoardTexture, 1000);
		batchComponent->CreateSubTexturedQuad({ 0,0,0 }, 0, { 1,1 ,1 }, glm::vec4(1), spriteSheet1);
		batchComponent->CreateSubTexturedQuad({ 2.5,2.5,-.75f }, 0, glm::vec3(5, 5, 1), glm::vec4(1), spriteSheet2);
		//uiComponent->CreateText("144 FPS", font, { -1.f,.5f,0 }, { 0,0,1 }, { 1,1,1,1 });
		//uiComponent->CreateText("a quick brown fox jumps over the lazy dog 0123456789=-+_)(*&^%$#@!AQUICKBROWNFOXJUMPSOVERTHELAZYDOG[]{}\|/?.,<>\"\\`~", font, { -1.f,.5f,0 }, { 0,0,1 }, { 0,1,0,1 });
		GEngine::ParticleProps props;
		props.ColorBegin = { 252/255.f, 49/255.f, 3/255.f, .4f};
		props.ColorEnd = { 252/255.f, 213/255.f, 96/255.f, .1f};
		props.LifeTime = 1.f;
		props.SizeBegin = .25f;
		props.SizeEnd = .15f;
		props.SizeVariation = .15f;
		props.Velocity = { 0.f, 0.f };
		props.VelocityVariation = { 2.f, 2.f };
		particles = GEngine::CreateGameObject<GEngine::ParticleSystem2DComponent>(props);
		mainEntity->AddComponent(particles);

		props.ColorBegin = { 252/255.f, 247/255.f, 96/255.f, .4f };
		particles2 = GEngine::CreateGameObject<GEngine::ParticleSystem2DComponent>(props);
		mainEntity->AddComponent(particles2);
	
		AddEntity(ent1);
		AddEntity(ent2);
		AddEntity(mainEntity);
		AddEntity(fpsEntity);
		AddEntity(e);

		
		GE_LOG_DEBUG("EntityId: {0}", mainEntity->hash);


	}
	GEngine::Ref < GEngine::Entity > ent1;
	GEngine::Ref < GEngine::Entity > ent2;

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
			//	if (GEngine::Input::IsKeyPressed(GE_KEY_UP))
				//	m_Rotation.x += m_CameraRotSpeed * m_CameraController->GetFOV() * timeStep.GetSeconds();
				//if (GEngine::Input::IsKeyPressed(GE_KEY_DOWN))
			//		m_Rotation.x -= m_CameraRotSpeed * m_CameraController->GetFOV() * timeStep.GetSeconds();
			//	if (GEngine::Input::IsKeyPressed(GE_KEY_LEFT))
			//		m_Rotation.y += m_CameraRotSpeed * m_CameraController->GetFOV() * timeStep.GetSeconds();
			//	if (GEngine::Input::IsKeyPressed(GE_KEY_RIGHT))
			//		m_Rotation.y -= m_CameraRotSpeed * m_CameraController->GetFOV() * timeStep.GetSeconds();

				if (GEngine::Mobile_Input::GetTouchCount() == 1) {
					std::unordered_map<uint64_t, GEngine::FTouchInfo> touches = GEngine::Mobile_Input::GetTouches();
					if (touches.begin()->first == m_lastTouchId) {
						if (abs(m_LastTouchPos.x - touches.begin()->second.x) > .1f ||
							abs(m_LastTouchPos.y - touches.begin()->second.y) > .1f) {

							glm::vec2 m_touchDir = { m_LastTouchPos.x - touches.begin()->second.x, m_LastTouchPos.y - touches.begin()->second.y };
							//m_touchDir = glm::normalize(m_touchDir);
							m_LastTouchPos = { touches.begin()->second.x, touches.begin()->second.y };
							m_Position.y += (m_CameraMoveSpeed * m_CameraController->GetFOV() * timeStep.GetSeconds()) * -m_touchDir.y / 10.f;
							m_Position.x += (m_CameraMoveSpeed * m_CameraController->GetFOV() * timeStep.GetSeconds()) * m_touchDir.x / 10.f;
						}

					}
				}

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
