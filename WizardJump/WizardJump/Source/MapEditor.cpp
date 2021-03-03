#include "MapEditor.hpp"
#include "DebugLayer.h"
#include "Environment/SpriteEntity.hpp"
#include "Lighting/LightComponent.h"
#include <CXML/CXML.hpp>
#include "Character/CharacterEntity.h"
#include "Character/CharacterController.hpp"
#include "Character/CharacterBody.hpp"
#include "Environment/BackgroundEntity.hpp"
#include "UI/DialogFrame.hpp"
#include "Tutorial/Tutorial.hpp"
#include "Environment/FogEntity.h"


static Ref<QuadColliderComponent> GetNodeAsQuadColliderComponent(Ref<Entity> e, const CXML::CXML_Node& n, size_t* _endPos = nullptr);
static std::string QuadColliderComponentToCXML(Ref<QuadColliderComponent> c);
static CXML cxml;

static std::string pauseCallback;
static std::string savePath = "Content/Scenes/Scene.scene";
static bool tutorial = false;

MapEditor::MapEditor(const char* id, Camera* camera) : Scene(id, camera)
{
	
}

MapEditor::~MapEditor()
{

}

void MapEditor::OnEvent(Event& e)
{
	m_CameraController->OnEvent(e);

	if (e.GetEventType() == EventType::KeyReleased) {
		GEngine::KeyReleasedEvent& _e = (GEngine::KeyReleasedEvent&)e;
		if (_e.GetKeyCode() == GE_KEY_F5) {
			LoadScene((char*)FileSystem::FileDataFromPath(savePath)->GetDataAsString());
			if (tutorial)
				RunTutorial();
		}
	}

}

Ref<CharacterEntity> MapEditor::characterEntity = nullptr;
static Ref<SpriteComponent> m_debugDrawer = nullptr;
Ref<Entity> fpsEnt;
Ref<UIComponent> uiComp;
Ref<Font> font;
std::string fpsId;

static double ts = 0;
static double ts1 = 0;

void MapEditor::OnUpdate(Timestep timestep)
{
	ts1 += timestep.GetMilliseconds();
	if (characterEntity) {
		Vector3f pos;
		float dist = GEngine::GEMath::distance(m_CameraController->GetPosition(), characterEntity->GetEntityPosition());
		if (dist > .1f) {
			pos = Vector3f::lerp(m_CameraController->GetPosition(), characterEntity->GetEntityPosition(), 10.f * timestep);
			m_CameraController->SetPosition(pos);
		}
		else {
			pos = characterEntity->GetEntityPosition();
			m_CameraController->SetPosition(pos);
		}
		
		ts1 = 0;

		
	}

	m_CameraController->OnUpdate(timestep);

#if !defined(GE_CONSOLE_APP)
	if (fpsId.size() > 0) {
		uiComp->RemoveText(fpsId);
	}
	fpsId = uiComp->CreateText(std::to_string((int)
		GEngine::Application::GetApp()->profile["FPS"]) + " fps", font, { -1, .9f,1 },
		{ 1,1,1 }, { 1,1,1,1 });
#else 
	ts += timestep.GetMilliseconds();

	if (ts > 16) {
		char ch[256] = { 0 };
		sprintf(ch, "Wizard Jump - FPS: %d | %.3f ms\0", (int)GEngine::Application::GetApp()->profile["FPS"], GEngine::Application::GetApp()->profile["Run"]);
		Application::GetApp()->GetWindow()->SetTitle(ch);
		ts = 0;
	}
#endif

	
}


static void GamePauseCallback(bool b) {
	
}



void MapEditor::OnBegin()
{
#if !defined(GE_CONSOLE_APP)
	fpsEnt = CreateGameObject<Entity>();
	AddEntity(fpsEnt);
	uiComp = CreateGameObject<UIComponent>();
	fpsEnt->AddComponent(uiComp);
	//font = GEngine::Font::Create("Content/Fonts/Wizard.ttf", 120.f);
	font = GEngine::Font::Create("Content/Fonts/arial.ttf", 120.f);
	font->LoadCharactersEN();
	std::u32string unicode = U"ٿ and څ then Ԭ $ next сука блять\n∞";
	font->LoadCharacters_u32((uint32_t*)unicode.data(), unicode.size());
	uiComp->CreateText_u32((uint32_t*)unicode.data(), unicode.size(), font, { -.5f,0,20 }, { 2.f,2.f,1 }, { 1,0,0,1 });
#endif
#if 0 && defined(GE_CONSOLE_APP)
	fpsEnt = CreateGameObject<Entity>();
	AddEntity(fpsEnt);
	uiComp = CreateGameObject<UIComponent>();
	fpsEnt->AddComponent(uiComp);
	//font = GEngine::Font::Create("Content/Fonts/Wizard.ttf", 120.f);
	font = GEngine::Font::Create("Content/Fonts/arial.ttf", 120.f);
	font->LoadCharactersEN();
	std::u32string unicode = U"ٿ and څ then Ԭ $ next сука блять\n∞";
	font->LoadCharacters_u32((uint32_t*)unicode.data(), unicode.size());
	uiComp->CreateText_u32((uint32_t*)unicode.data(), unicode.size(), font, { -.5f,0,20 }, { 2.f,2.f,1 }, { 1,0,0,1 });
#endif
	/*
	fpsEnt = CreateGameObject<Entity>();
	AddEntity(fpsEnt);
	uiComp = CreateGameObject<UIComponent>();
	font = GEngine::Font::Create("Content/Fonts/arial.ttf", 120.f);
	fpsEnt->AddComponent(uiComp);
	//font->LoadCharactersEN();
	font->LoadCharacter_u32(0x0686);
	uint32_t str[1] = { 0x0686 };
	uiComp->CreateText_u32(str, 1, font, { 0,0,20 }, { 2.f,2.f,1 }, { 1,0,0,1 });
	*/

	DebugLayer::showScene = false;
	camera = m_CameraController->GetCamera().get();
	GEngine::Application::GetApp()->SetTargetCameraController(m_CameraController.get());
	GEngine::Application::GetApp()->SetTargetCamera(camera);


	if (!GEngine::Application::DebugTools()) {
		LoadScene((char*)FileSystem::FileDataFromPath(savePath)->GetDataAsString());
		GEngine::Application::GetApp()->GetTargetCameraController()->SetCameraZoom(10.f);
		GEngine::Application::GetApp()->GetTargetCameraController()->SetPosition(Vector3f(0.f));
		//Application::PauseGame();
		//Application::ResumeGame();
		if (tutorial) RunTutorial();
	}
	else {
		pauseCallback = Application::GetApp()->AddOnGamePauseCallback([this](bool b) {
			if (b) {
				tutorial = false;
				LoadScene((char*)FileSystem::FileDataFromPath(savePath)->GetDataAsString());
			}
			else {
				if (Application::GetApp()->IsRunning()) {
					if (tutorial) RunTutorial();
					GEngine::Application::GetApp()->GetTargetCameraController()->SetCameraZoom(10.f);
					GEngine::Application::GetApp()->GetTargetCameraController()->SetPosition(Vector3f(0.f));
					SaveScene(savePath);
				}
			}
			});


		Application::PauseGame();
	}
	return;
};

static void Inspector();

void MapEditor::OnEnd()
{
	if (GEngine::Application::DebugTools())
		Application::GetApp()->RemoveOnGamePauseCallback(pauseCallback);
}

void MapEditor::OnImGuiRender()
{
	SceneMenu();
	Inspector();
}

void MapEditor::OnLoad()
{
	SetupCamera();
}

void MapEditor::RunTutorial()
{
	Tutorial::CreateMainTutorial(dynamic_pointer_cast<Scene>(self.lock()), characterEntity);
}

void MapEditor::OnUnload()
{
	m_CameraController.reset();
}

void MapEditor::SetupCamera()
{
	m_CameraController = std::unique_ptr<GEngine::Orthographic_CameraController>(new GEngine::Orthographic_CameraController(
		(float)GEngine::Application::GetWidth() / (float)GEngine::Application::GetHeight()));

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
			
		});
	m_CameraController->SetOnEventFn([this](GEngine::Event& e) {
		GEngine::EventDispatcher dispatcher(e);

		dispatcher.Dispatch<GEngine::MouseScrolledEvent>([this](GEngine::MouseScrolledEvent& e) {
			if (Application::InputEnabled())
				m_CameraController->OnCameraZoom(e.GetXOffset(), e.GetYOffset(), .1f);
			return false;
			});

		dispatcher.Dispatch<GEngine::KeyPressedEvent>([this](GEngine::KeyPressedEvent& e) {
			if (!Application::IsGamePaused()) return false;
			const float m_CameraMoveSpeed = .1f;
			const float m_CameraRotSpeed = 180.f;
			Vector3f m_Position = m_CameraController->GetPosition();
			Vector3f m_Rotation = m_CameraController->GetRotation();
			if (GEngine::Input::IsKeyPressed(GE_KEY_A))
				m_Position.x -= m_CameraMoveSpeed * m_CameraController->GetFOV() ;
			if (GEngine::Input::IsKeyPressed(GE_KEY_D))
				m_Position.x += m_CameraMoveSpeed * m_CameraController->GetFOV();
			if (GEngine::Input::IsKeyPressed(GE_KEY_LEFT_SHIFT))
				m_Position.z -= m_CameraMoveSpeed * m_CameraController->GetFOV();
			if (GEngine::Input::IsKeyPressed(GE_KEY_LEFT_CONTROL))
				m_Position.z += m_CameraMoveSpeed * m_CameraController->GetFOV();
			if (GEngine::Input::IsKeyPressed(GE_KEY_W))
				m_Position.y += m_CameraMoveSpeed * m_CameraController->GetFOV();
			if (GEngine::Input::IsKeyPressed(GE_KEY_S))
				m_Position.y -= m_CameraMoveSpeed * m_CameraController->GetFOV();
			if (GEngine::Input::IsKeyPressed(GE_KEY_Q))
				m_Rotation.z += m_CameraRotSpeed * m_CameraController->GetFOV();
			if (GEngine::Input::IsKeyPressed(GE_KEY_E))
				m_Rotation.z -= m_CameraRotSpeed * m_CameraController->GetFOV();

			m_CameraController->SetPosition(m_Position);
			m_CameraController->SetRotation(m_Rotation);

			m_CameraController->OnUpdate(16.f / 1000.f);

			return false;
			});
		});
}


static GEngine::Ref<GEngine::GameObject> hashSelected = nullptr;
static GEngine::Ref<GEngine::GameObject> compSelected = nullptr;
static uint64_t intHashSelected = 0;
static bool b_component = false;
static bool b_addObj = false;
static bool b_showpopup = false;
static std::string entityCreate = "Entity";
static std::string componentCreate = "SpriteComponent";
template <typename T, typename ... Args>
static Ref<GameObject> mapEditorTemplate(uint64_t id, Args&& ... args) { Ref<Entity> e = CreateGameObject_ID<T>(id, std::forward<Args>(args)...); SceneManager::GetCurrentScene()->AddEntity(e); return e; }
unordered_map<std::string, std::function<Ref<GameObject>(uint64_t)>> MapEditor::entityMap = { { "Entity", [](uint64_t id) { return mapEditorTemplate<Entity>(id); }}, {"CharacterEntity", [](uint64_t id) { Ref<CharacterEntity> e = CreateGameObject_ID<CharacterEntity>(id); SceneManager::GetCurrentScene()->AddEntity(e); characterEntity = e; return e; }  }, { "BackgroundEntity", [](uint64_t id) {return mapEditorTemplate<BackgroundEntity>(id); } }, { "Tutorial", [](uint64_t id) {tutorial = true; return mapEditorTemplate<Tutorial>(id); } },
	{"FogEntity", [](uint64_t id) {return mapEditorTemplate<FogEntity>(id); }}
};
static unordered_map<std::string, std::function<Ref<GameObject>(uint64_t)>> componentMap = { { "SpriteComponent", [](uint64_t id) { dynamic_pointer_cast<Entity>(hashSelected)->AddComponent(CreateGameObject_ID<SpriteComponent>(id)); return nullptr; } }, {"LightComponent", [](uint64_t id) { dynamic_pointer_cast<Entity>(hashSelected)->AddComponent(CreateGameObject_ID<LightComponent>(id)); return nullptr; }}, {"QuadColliderComponent", [](uint64_t id) {dynamic_pointer_cast<Entity>(hashSelected)->AddComponent(CreateGameObject_ID<QuadColliderComponent>(id, false, true)); return nullptr; }}
};

void MapEditor::SceneMenu()
{
#ifdef GE_CONSOLE_APP
	if (ImGui::BeginPopupModal("Create Object", &b_addObj, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Type:");
		if (!ImGui::IsWindowHovered()) {
			if (ImGui::IsKeyPressed(GE_KEY_ESCAPE)) {
				b_addObj = false;
			}
		}
		ImGui::SameLine();
		if (ImGui::BeginCombo("", (!b_component ? entityCreate.c_str() : componentCreate.c_str()))) {
			const unordered_map<std::string, std::function<Ref<GameObject>(uint64_t)>>& map = !b_component ? entityMap : componentMap;
			for (const std::pair<std::string, std::function<Ref<GameObject>(uint64_t)>>& p : map) {
				bool selected = (hashSelected == nullptr ? entityCreate : componentCreate) == p.first;
				if (ImGui::Selectable(p.first.c_str(), selected)) {
					(!b_component ? entityCreate : componentCreate) = p.first;
				}
				if (selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::SetCursorPosX( (ImGui::GetWindowSize().x / 2.f) - 100.f);
		if (ImGui::Button(!b_component ? "Add Entity" : "Add Component", { 200,30 })) {
			bool ent = !b_component;
			if (ent) {
				entityMap[entityCreate](0);
			}
			else {
				componentMap[componentCreate](0);
				b_component = false;
			}
			b_addObj = false;
		}
		ImGui::EndPopup();
	}

	if (b_addObj) {
		ImGui::OpenPopup("Create Object");
	}


	ImGui::Begin("Scene Hierarchy"); {
		if (ImGui::Button("Add")) {
			b_addObj = true;
			b_component = false;
			/*			if (dynamic_pointer_cast<Component>(hashSelected) != nullptr) {
				if (dynamic_pointer_cast<SpriteComponent>(hashSelected)) {
					Ref<SpriteComponent> s = dynamic_pointer_cast<SpriteComponent>(hashSelected);
					s->CreateQuad({ 0,0, 0 });
				}
			}
			*/

		}

		if (hashSelected != nullptr) {
			ImGui::SameLine();
			if (ImGui::Button("Destroy")) {
				GEngine::Ref<GEngine::Entity> __e = static_pointer_cast<GEngine::Entity>(hashSelected);
				__e->Destroy();
				
				hashSelected = nullptr;
				compSelected = nullptr;
				b_component = false;
				ImGui::End();
				return;
			}
		}

		

		if (Application::IsGamePaused()) {
			ImGui::SameLine();
			if (ImGui::Button("Save")) {
				SaveScene(savePath);
			}

			ImGui::SameLine();
			if (ImGui::Button("Load")) {

				

				LoadScene((char*)FileSystem::FileDataFromPath(savePath)->GetDataAsString());
			}
			ImGui::SameLine();
			char ch[256] = { 0 };
			memcpy(ch, &savePath[0], savePath.size());
			if (ImGui::InputText("##hidelabel", ch, ImGuiInputTextFlags_EnterReturnsTrue)) {
				savePath = ch;
			}
		}

		ImGui::Separator();

		//ImGui::SetNextItemOpen(true, ImGuiCond_Always);
		if (true)
		{
			std::unordered_map<uint64_t, GEngine::Ref<GEngine::Entity>> entities = GEngine::SceneManager::GetCurrentScene()->GetEntities();
			std::vector<uint64_t> hashes;
			for (const std::pair<uint64_t, GEngine::Ref<GEngine::Entity>>& e : entities) {
				hashes.push_back(e.first);
			}

			std::sort(hashes.begin(), hashes.end());

			for (const uint64_t& hash : hashes)
			{
				std::pair<uint64_t, Ref<Entity>> e = { hash, entities[hash] };
				if (e.second->m_tag == "__EditorEntity")
					continue;
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.8, .8, .8, 1));
				ImGuiTreeNodeFlags entity_base_flags = ImGuiTreeNodeFlags_Leaf;
				if (hashSelected == e.second) {
					entity_base_flags |= ImGuiTreeNodeFlags_Selected;
				}
				
				char ch[9] = { 0 };
				memcpy(ch, &e.first, sizeof(sizeof(uint64_t)));

				bool show_e = ImGui::TreeNodeEx((void*)(intptr_t)e.first, entity_base_flags, "%s : %s (%ull)", e.second->m_tag.c_str(), typeid(*e.second.get()).name(), e.first);
				
				if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1)) {
					if (hashSelected != e.second) {
						b_component = false;
						hashSelected = e.second;
					}
					else if (!ImGui::IsItemClicked(1)) {
						b_component = false;
						hashSelected = nullptr;
					}
				}
				
				if (hashSelected == e.second) {
					if (ImGui::BeginPopupContextItem("RenameEntity")) {

					char ch[255] = { 0 };
					if (ImGui::InputText("Rename Entity", ch, sizeof(ch), ImGuiInputTextFlags_EnterReturnsTrue)) {
						e.second->m_tag = ch;
						ImGui::CloseCurrentPopup();
					}

					if (Input::IsKeyPressed(GE_KEY_ESCAPE)) {
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
				}
				bool skip_c = false;

				
				if (show_e) {
					ImGui::TreePop();
				}
				ImGui::PopStyleColor(1);
				
			}

			//ImGui::TreePop();
		}


	}
	ImGui::End();
#endif
}

static void Inspector() {
#ifdef GE_CONSOLE_APP
	bool show_e = false;
	bool skip_c = false;
	Ref<Entity> e = nullptr;
	e = dynamic_pointer_cast<Entity>(hashSelected);
	ImGui::Begin("Inspector");
	if (e)
	{
		ImGui::Text("%s", e->m_tag.c_str());
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(8.f, 8.f, 8.f, 1));
		std::unordered_map<uint64_t, GEngine::Ref<GEngine::Component>> components = e->GetComponents();

		if (auto __e = dynamic_pointer_cast<BackgroundEntity>(e)) {
			skip_c = true;
			for (auto p : __e->GetBackgrounds()) {
				bool show_pp = ImGui::TreeNodeEx((void*)(intptr_t)std::hash<std::string>{}(p.first), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick, "%s bg", p.first.c_str());
				if (ImGui::BeginPopupContextItem((std::string("backGroundDupe") + p.first).c_str())) {

					if (ImGui::Button("Duplicate")) {
						__e->AddParalaxBackground(p.first + "_Copy", p.second.texture.lock(), p.second.scale, p.second.speed, p.second.zOrder, p.second.offset);
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
				if (show_pp) {

					bool recreate = false;
					char ch[255] = { 0 };
					ImGui::Text("Name: ");
					ImGui::SameLine();
					memcpy(ch, &p.first[0], p.first.size());
					if (ImGui::InputText("##hidelabel1", ch, sizeof(ch), ImGuiInputTextFlags_EnterReturnsTrue)) {
						if (p.first != ch) {
							recreate = true;
						}
					}
					ImGui::Text("Texture: ");
					ImGui::SameLine();
					char ch1[255] = { 0 };
					memcpy(ch1, &p.second.texture.lock()->GetName()[0], p.second.texture.lock()->GetName().size());
					if (ImGui::InputText("##hidelabel2", ch1, sizeof(ch1), ImGuiInputTextFlags_EnterReturnsTrue)) {
						if (p.second.texture.lock()->GetName() != ch1) {
							recreate = true;
						}
					}

					ImGui::Text("Texture Flags: ");
					ImGui::SameLine();
					int flags = p.second.texture.lock()->GetFlags();
					if (ImGui::InputInt("##hidelabelflag", &flags)) {
						if (p.second.texture.lock()->GetFlags() != flags)
							recreate = true;
					}

					float sc[2] = { p.second.scale.x, p.second.scale.y };
					ImGui::Text("Scale: ");
					ImGui::SameLine();
					if (ImGui::InputFloat2("##hidelabel3", sc)) {
						if (p.second.offset != sc) {
							recreate = true;
						}
					}

					float sp = p.second.speed;

					ImGui::Text("Speed: ");
					ImGui::SameLine();
					if (ImGui::InputFloat("##hidelabel4", &sp)) {
						if (p.second.speed != sp) {
							recreate = true;
						}
					}

					float zO = p.second.zOrder;
					ImGui::Text("zOrder: ");
					ImGui::SameLine();
					if (ImGui::InputFloat("##hidelabel5", &zO)) {
						if (p.second.zOrder != zO) {
							recreate = true;
						}
					}

					float of[2] = { p.second.offset.x, p.second.offset.y };
					ImGui::Text("Offset: ");
					ImGui::SameLine();
					if (ImGui::InputFloat2("##hidelabel6", of)) {
						if (p.second.offset != of) {
							recreate = true;
						}
					}

					if (ImGui::Button("Destroy")) {
						__e->RemoveParalaxBackground(p.first);
					}

					if (recreate) {
						__e->RemoveParalaxBackground(p.first);
						__e->AddParalaxBackground(ch, Texture2D::Create(ch1, flags), sc, sp, zO, of);
					}

					ImGui::TreePop();
				}
			}
			if (ImGui::Button("Add Background")) {
				__e->AddParalaxBackground("background", Texture2D::Create("batchBlank"), { 1,1 }, 1, 0, { 0,0 });
			}
			

		}

		for (const std::pair<uint64_t, GEngine::Ref<GEngine::Component>>& c : components) {
			if (skip_c) continue;

			ImGuiTreeNodeFlags component_base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
			if (compSelected == c.second) {
				component_base_flags |= ImGuiTreeNodeFlags_Selected;
			}
			bool show_c = ImGui::TreeNodeEx((void*)(intptr_t)c.first, component_base_flags, "%s : %s (%ju)", c.second->m_tag.c_str(), typeid(*c.second.get()).name(), c.first);
			if (ImGui::IsItemClicked()) {
				compSelected = c.second;
				intHashSelected = 0;
				if (m_debugDrawer)
					m_debugDrawer->ClearQuads();
			}

			if (Ref<LightComponent> lightC = dynamic_pointer_cast<LightComponent>(c.second); lightC != nullptr) {
				if (ImGui::BeginPopupContextItem("Create Light")) {

					if (ImGui::Button("Create Quad Light")) {
						lightC->AddQuadLight({ 0,0 }, 1, { 1,1 }, { 1,1,1,.99f });
					}

					if (ImGui::Button("Create Cirlce Light")) {
						lightC->AddCircleLight({ 0,0 }, 1, { 1,1 }, { 1,0,0,1.f });
					}

					ImGui::EndPopup();
				}
			}

			if (Ref<GEngine::QuadColliderComponent> quad = dynamic_pointer_cast<QuadColliderComponent>(c.second); quad != nullptr) {
				
				if (ImGui::BeginPopupContextItem("QuadColliderCreation")) {

					if (ImGui::Button("Create Quad")) {
						quad->CreateQuad({ 0,0 }, { 1,1 }, 1.f, 0.f, "Quad");
						//quad->WakeBody();
					}

					if (ImGui::Button("Create Circle")) {
						quad->CreateCircle({ 0,0 }, { 1,1 }, 1.f, 0.f, "Circle");
						//quad->WakeBody();
					}

					ImGui::EndPopup();
				}
			}

			if (GEngine::Ref<GEngine::SpriteComponent> spriteComp = dynamic_pointer_cast<GEngine::SpriteComponent>(c.second); spriteComp != nullptr) {


				

				if (ImGui::BeginPopupContextItem("ShowCreatButton")) {

					if (ImGui::Button("Create Textured Quad")) {
						spriteComp->CreateQuad({ 0,0,0 });
					}

					if (ImGui::Button("Create SubTextured Quad")) {
						spriteComp->CreateSubTexturedQuad({ 0,0,0 }, 0, { 1,1,1 }, { 1,1,1,1 }, SubTexture2D::CreateFromCoords(Texture2D::Create("batchBlank"), { 0,0 }, { 1,1 }));
					}

					ImGui::EndPopup();
				}

			}

			if (show_c) {
				if (Ref<LightComponent> lightC = dynamic_pointer_cast<LightComponent>(c.second); lightC != nullptr) {
					
					int i = 0;
					while (i <= 1) {
						std::vector<ShapeID> ids = i == 0 ? lightC->GetQuadLights() : lightC->GetCircleLights();

						for (const ShapeID& id : ids) {
							ImGuiTreeNodeFlags sprite_component_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
							if (intHashSelected == id) {
								sprite_component_flags |= ImGuiTreeNodeFlags_Selected;
							}

							bool show_sid = ImGui::TreeNodeEx((void*)(intptr_t)id, sprite_component_flags, "%s, %llu", (i == 0 ? "Quad Light" : "Circle Light"), id);
							Ref<BatchRenderer> batchRender = (i == 0 ? lightC->GetQuadRenderer() : lightC->GetCircleRenderer());
							if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1)) {
								intHashSelected = id;
							}
							if (show_sid) {
								Vector3f p = batchRender->GetShapePosition(id) - e->GetEntityPosition();
								float v[3];
								memcpy(v, p.data(), sizeof(float) * 3);
								ImGui::Text("Position: ");
								ImGui::SameLine();
								if (ImGui::InputFloat3("Pos", v, 4)) {
									if (Vector3f(v) != p) {
										batchRender->SetPosition(id, (Vector2f(v) + e->GetEntityPosition().xy()));
										if (v[2] != p.z) {
											batchRender->SetZOrder(id, v[2] + e->GetEntityPosition().z);
										}
									}
								}
								float _rot = batchRender->GetShapeRotation(id);
								float rot = _rot - e->GetEntityRotation().z;
								ImGui::Text("Rotation: ");
								ImGui::SameLine();
								ImGui::InputFloat("rot", &rot);
								if (rot != _rot - e->GetEntityRotation().z) {
									batchRender->SetRotation(id, rot + e->GetEntityRotation().z);
								}
								float _s[2];
								ImGui::Text("Scale: ");
								ImGui::SameLine();
								Vector2f s = batchRender->GetShapeScale(id);
								Vector2f __s = e->GetEntityScale().xy();
								s = { s.x / __s.x,s.y / __s.y };
								memcpy(_s, s.data(), sizeof(float) * 2);
								if (ImGui::InputFloat2("scale", _s, 4)) {
									if (s != Vector2f(_s)) {
										batchRender->SetScale(id, Vector2f(_s) * e->GetEntityScale().xy());
									}
								}
								ImGui::Text("Color: ");
								ImGui::SameLine();
								Vector4f _c = batchRender->GetShapeColor(id);
								float __c[4];
								memcpy(__c, _c.data(), sizeof(float) * 4);
								if (ImGui::InputFloat4("sColr", __c)) {
									if (Vector4f(__c) != _c) {
										batchRender->SetColor(id, __c);
									}
								}
								if (i == 0) {
									ImGui::Text("Texture: ");
									ImGui::SameLine();
									std::string tex = batchRender->GetShapeTexture(id)->GetName();
									char ch[255] = { 0 };
									memcpy(ch, &tex[0], tex.size() * sizeof(char));
									if (ImGui::InputText("texStr", ch, 255, ImGuiInputTextFlags_EnterReturnsTrue)) {
										batchRender->SetTexture(id, Texture2D::Create(std::string(ch), TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest));
									}
								}
								if (ImGui::Button("Destroy")) {
									if (i == 0) lightC->RemoveQuadLight(id);
									else lightC->RemoveCircleLight(id);
								}
								ImGui::TreePop();
							}

							if (intHashSelected == id && m_debugDrawer) {
								m_debugDrawer->ClearQuads();
								m_debugDrawer->CreateQuad(Vector3f(batchRender->GetShapePosition(id).xy(), 1000), batchRender->GetShapeRotation(id), { batchRender->GetShapeScale(id), 1 }, { 0,1,0,.25f });
							}
						}
						i++;
					}



				}
				if (GEngine::Ref<GEngine::Transform> transform = dynamic_pointer_cast<GEngine::Transform>(c.second); transform != nullptr) {
					float v[3];
					

					if (!transform) return;

					v[0] = transform->GetPosition().x;
					v[1] = transform->GetPosition().y;
					v[2] = transform->GetPosition().z;
					ImGui::Text("Position: ");
					ImGui::SameLine();
					if (ImGui::InputFloat3("tPos", v, 4)) {
						if (v[0] != transform->GetPosition().x || v[1] != transform->GetPosition().y || v[2] != transform->GetPosition().z) {
							e->SetEntityPosition({ v[0], v[1], v[2] });
						}
					}
					float r[3];
					memcpy(r, e->GetEntityRotation().data(), sizeof(float) * 3);
					ImGui::Text("Rotation: ");
					ImGui::SameLine();
					if (ImGui::InputFloat3("eRot", r)) {
						if (transform->GetRotation() != Vector3f(r)) {
							e->SetEntityRotation(r);
						}
					}

					float s[3];
					ImGui::Text("Scale: ");
					ImGui::SameLine();
					Vector3f _s = transform->GetScale();
					memcpy(s, _s.data(), sizeof(float) * 3);
					if (ImGui::InputFloat3("tScale", s)) {
						if (_s != Vector3f(s)) {
							e->SetEntityScale(Vector3f(s));
						}
					}

				}
				if (GEngine::Ref<GEngine::QuadColliderComponent> collider = dynamic_pointer_cast<GEngine::QuadColliderComponent>(c.second); collider != nullptr) {
					
					if (collider == nullptr) return;

					bool phys = collider->HasPhysics();
					ImGui::Text("Physics: ");
					ImGui::SameLine();
					if (ImGui::Checkbox("##hidelabelphysics", &phys)) {
						collider->SetPhysics(phys);

						Ref<Entity> ec = collider->GetEntity();
						std::string xcml = QuadColliderComponentToCXML(collider);
						collider->Destroy();
						collider = GetNodeAsQuadColliderComponent(ec, cxml.GetNext(xcml.c_str()));
					}
					bool dyn = collider->IsDynamic();
					ImGui::Text("Dynamic: ");
					ImGui::SameLine();
					if (ImGui::Checkbox("##hidelabeldynamic", &dyn)) {
						collider->SetDynamic(dyn);

						Ref<Entity> ec = collider->GetEntity();
						std::string xcml = QuadColliderComponentToCXML(collider);
						collider->Destroy();
						collider = GetNodeAsQuadColliderComponent(ec, cxml.GetNext(xcml.c_str()));
					}

					ImGui::Text("Velocity: ");
					ImGui::SameLine();
					float vl[2];
					Vector2f _vl = collider->GetLinearVelocity();
					memcpy(vl, _vl.data(), sizeof(float) * 2);
					ImGui::InputFloat2("__vl", vl);
					if (_vl != Vector2f(vl)) {
						collider->SetVelocity(vl[0], vl[1]);
					}
					bool b = collider->GetFixedRotation();
					ImGui::Text("Fixed Rotation: ");
					ImGui::SameLine();
					if (ImGui::Checkbox("##hidelabel", &b)) {
						collider->SetFixedRotation(b);
					}

					if (ImGui::Button("Wake Bodies")) {
						collider->WakeBody(true);
					}
					ImGui::SameLine();
					if (ImGui::Button("Sleep Bodies")) {
						collider->WakeBody(false);
					}

					for (std::pair<const ColliderID, FColliderQuad >& c : collider->GetColliders()) {
						bool show_sid = ImGui::TreeNodeEx((void*)(intptr_t)c.first, component_base_flags, "%s - %s : %llu", c.second.tag.c_str(), (c.second.quad ? "QuadCollider" : "CircleCollider"), c.first);
						if (show_sid) {
							ImGui::Text("Tag: ");
							ImGui::SameLine();
							char buff[255] = { 0 };
							memcpy(buff, &c.second.tag[0], c.second.tag.size());
							if (ImGui::InputText("__", buff, sizeof(buff), ImGuiInputTextFlags_EnterReturnsTrue)) {
								c.second.tag = std::string(buff);
							}
							ImGui::Text("Position: ");
							ImGui::SameLine();
							float p[2];
							memcpy(p, c.second.position.data(), sizeof(float) * 2);
							ImGui::InputFloat2("__f", p);
							if (Vector2f(p) != c.second.position) {
								c.second.position = Vector2f(p);
							}
							ImGui::Text("Rotation: ");
							ImGui::SameLine();
							ImGui::InputFloat("__r", &c.second.rotation);

							ImGui::Text("Scale: ");
							ImGui::SameLine();
							float s[2];
							memcpy(s, c.second.scale.data(), sizeof(float) * 2);
							ImGui::InputFloat2("__s", s);
							if (Vector2f(s) != c.second.scale) {
								c.second.scale = Vector2f(s);
							}
							ImGui::Text("Mass: ");
							ImGui::SameLine();
							ImGui::InputFloat("__m", &c.second.mass);
							ImGui::Text("Bounce: ");
							ImGui::SameLine();
							float bounce = c.second.bounce;
							if (ImGui::InputFloat("__bo", &bounce)) {
								collider->SetBounce(c.first, bounce);
							}
							if (ImGui::Button("Destroy")) {
								collider->DestroyQuad(c.first);
							}

							ImGui::TreePop();
						}

					}



				}
				if (GEngine::Ref<GEngine::SpriteComponent> spriteComp = dynamic_pointer_cast<GEngine::SpriteComponent>(c.second); spriteComp != nullptr) {


					

					std::vector<ShapeID> ids = spriteComp->GetQuads();
					Ref<BatchRenderer> batchRender = spriteComp->GetBatchRenderer();
					int counter = 0;
					std::sort(ids.begin(), ids.end());
					for (const ShapeID& id : ids) {
						ImGuiTreeNodeFlags sprite_component_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
						if (intHashSelected == id) {
							sprite_component_flags |= ImGuiTreeNodeFlags_Selected;
						}
						bool show_sid = ImGui::TreeNodeEx((void*)(intptr_t)ids[counter++], sprite_component_flags, "%s - %llu", batchRender->GetShapeTexture(id)->GetName().c_str(), id);

						if (ImGui::IsItemClicked() || ImGui::IsItemClicked(1)) {
							if (intHashSelected == id && ImGui::IsItemClicked(0)) { if (m_debugDrawer) m_debugDrawer->ClearQuads(); intHashSelected = 0; }
							else intHashSelected = id;
						}

						if (intHashSelected == id) {
							if (ImGui::BeginPopupContextItem("spriteComponentDupe")) {

								if (ImGui::Button("Duplicate")) {

									if (batchRender->GetShapeSubTexture(id) != nullptr) {
										Vector2f s = batchRender->GetShapeScale(id);
										Vector2f __s = e->GetEntityScale().xy();
										s = { s.x / __s.x,s.y / __s.y };
										
										Ref<SubTexture2D> subT = batchRender->GetShapeSubTexture(id);
										subT = SubTexture2D::CreateFromCoords(subT->GetTexture(), subT->m_coords, subT->m_cellSize, subT->m_spriteSize);
										spriteComp->CreateSubTexturedQuad(batchRender->GetShapePosition(id) - e->GetEntityPosition(), batchRender->GetShapeRotation(id) - e->GetEntityRotation().z, { s, 1 }, batchRender->GetShapeColor(id), subT, batchRender->GetTextureScale(id));
									}
									else {
										Vector2f s = batchRender->GetShapeScale(id);
										Vector2f __s = e->GetEntityScale().xy();
										s = { s.x / __s.x,s.y / __s.y };
										spriteComp->CreateQuad(batchRender->GetShapePosition(id) - e->GetEntityPosition(), batchRender->GetShapeRotation(id) - e->GetEntityRotation().z, { s, 1 }, batchRender->GetShapeColor(id), batchRender->GetShapeTexture(id), batchRender->GetTextureScale(id));
									}

									ImGui::CloseCurrentPopup();
								}

								ImGui::EndPopup();
							}
						}

						if (intHashSelected == id && m_debugDrawer) {
							m_debugDrawer->ClearQuads();
							m_debugDrawer->CreateQuad(Vector3f(batchRender->GetShapePosition(id).xy(), 1000), batchRender->GetShapeRotation(id), { batchRender->GetShapeScale(id), 1}, { 0,1,0,.25f });
						}

						if (show_sid) {
							Vector3f p = batchRender->GetShapePosition(id) - e->GetEntityPosition();
							float v[3];
							memcpy(v, p.data(), sizeof(float) * 3);
							ImGui::Text("Position: ");
							ImGui::SameLine();
							if (ImGui::InputFloat3("Pos", v, 4)) {
								if (Vector3f(v) != p) {
									spriteComp->SetPosition(id, { v[0], v[1] });
									if (v[2] != p.z) {
										spriteComp->SetZOrder(id, v[2]);
									}
								}
							}
							float _rot = batchRender->GetShapeRotation(id);
							float rot = _rot - e->GetEntityRotation().z;
							ImGui::Text("Rotation: ");
							ImGui::SameLine();
							ImGui::InputFloat("rot", &rot);
							if (rot != _rot - e->GetEntityRotation().z) {
								spriteComp->SetRotation(id, rot);
							}
							float _s[2];
							ImGui::Text("Scale: ");
							ImGui::SameLine();
							Vector2f s = batchRender->GetShapeScale(id);
							Vector2f __s = e->GetEntityScale().xy();
							s = { s.x / __s.x,s.y / __s.y };
							memcpy(_s, s.data(), sizeof(float) * 2);
							if (ImGui::InputFloat2("scale", _s, 4)) {
								if (s != Vector2f(_s)) {
									spriteComp->SetQuadScale(id, Vector2f(_s));
								}
							}
							ImGui::Text("Color: ");
							ImGui::SameLine();
							Vector4f _c = batchRender->GetShapeColor(id);
							float __c[4];
							memcpy(__c, _c.data(), sizeof(float) * 4);
							if (ImGui::InputFloat4("sColr", __c)) {
								if (Vector4f(__c) != _c) {
									batchRender->SetColor(id, __c);
								}
							}

							ImGui::Text("Texture Scale: ");
							ImGui::SameLine();

							float _ts[2];
							Vector2f __ts = batchRender->GetTextureScale(id);
							memcpy(_ts, __ts.data(), sizeof(float) * 2);
							if (ImGui::InputFloat2("ttscale", _ts)) {
								if (__ts != Vector2f(_ts)) {
									batchRender->SetTextureScale(id, _ts);
								}
							}

							if (batchRender->GetShapeSubTexture(id)) {
								uint64_t __id = id + 1;
								bool show_subTId = ImGui::TreeNodeEx((void*)(intptr_t)__id, component_base_flags, "Sub Texture: %s", batchRender->GetShapeSubTexture(id)->GetTexture()->GetName().c_str());
								if (show_subTId) {
									bool change = false;
									Ref<SubTexture2D> subT = batchRender->GetShapeSubTexture(id);
									std::string tex = subT->GetTexture()->GetName();
									char ch[512] = { 0 };
									memcpy(ch, &tex[0], tex.size() * sizeof(char));
									if (ImGui::InputText("texStr", ch, 255, ImGuiInputTextFlags_EnterReturnsTrue)) {
										change = true;
									}

									int fl = subT->GetTexture()->GetFlags();
									if (ImGui::InputInt("__flags", &fl)) {
										if (fl != subT->GetTexture()->GetFlags())
											change = true;
									}

									float coords[2];
									memcpy(coords, subT->m_coords.data(), sizeof(float) * 2);
									ImGui::Text("Coords: ");
									ImGui::SameLine();
									if (ImGui::InputFloat2("scoords", coords)) {
										subT->SetCoords(coords, subT->m_cellSize, subT->m_spriteSize);
										change = true;
									}

									float subSiz[2];
									memcpy(subSiz, subT->m_cellSize.data(), sizeof(float) * 2);
									ImGui::Text("Cell Size: ");
									ImGui::SameLine();
									if (ImGui::InputFloat2("subCell", subSiz)) {
										subT->SetCoords(subT->m_coords, subSiz);
										change = true;
									}

									float spSize[2];
									memcpy(spSize, subT->m_spriteSize.data(), sizeof(float) * 2);
									ImGui::Text("Sprite Size: ");
									ImGui::SameLine();
									if (ImGui::InputFloat2("sizeCell", spSize)) {
										subT->SetCoords(subT->m_coords, subT->m_cellSize, spSize);
										change = true;
									}

									if (change) {
										subT = SubTexture2D::CreateFromCoords(Texture2D::Create(std::string(ch), 192),
											subT->m_coords, subT->m_cellSize, subT->m_spriteSize);
										batchRender->SetSubTexture(id, subT);
									}
									ImGui::TreePop();
								}

							}
							else {
								ImGui::Text("Texture: ");
								ImGui::SameLine();
								std::string tex = batchRender->GetShapeTexture(id)->GetName();
								char ch[255] = { 0 };
								memcpy(ch, &tex[0], tex.size() * sizeof(char));
								if (ImGui::InputText("texStr", ch, 255, ImGuiInputTextFlags_EnterReturnsTrue)) {
									batchRender->SetTexture(id, Texture2D::Create(std::string(ch), TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest));
								}
							}


							if (ImGui::Button("Destroy")) {
								spriteComp->RemoveQuad(id);
							}

							ImGui::TreePop();
						}
					}

				}
				ImGui::TreePop();
			}
		}
		ImGui::PopStyleColor(1);
	}
	
	if (hashSelected) {
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::Separator();
		if (ImGui::Button("Add Component")) {
			b_component = true;
			b_addObj = true;
		}
	}

	if (compSelected) {
		if (dynamic_pointer_cast<Transform>(compSelected) == nullptr) {
			ImGui::SameLine();
			if (ImGui::Button("Destroy Component")) {

				dynamic_pointer_cast<Component>(compSelected)->Destroy();
				compSelected = nullptr;
			}
		}
	}
		
	ImGui::End();
#endif
}


static std::string Vec3fToCXML(const Vector3f& v) {
	return "<float> " + std::to_string(v.x) + " </float>\n" + "<float> " + std::to_string(v.y) + " </float>\n" + "<float> " + std::to_string(v.z) + " </float>\n";
}
static std::string Vec4fToCXML(const Vector4f& v) {
	return "<float> " + std::to_string(v.x) + " </float>\n" + "<float> " + std::to_string(v.y) + " </float>\n" + "<float> " + std::to_string(v.z) + " </float>\n" + "<float> " + std::to_string(v.w) + " </float>\n";
}
static std::string Vec2fToCXML(const Vector2f& v) {
	return "<float> " + std::to_string(v.x) + " </float>\n" + "<float> " + std::to_string(v.y) + " </float>\n";
}

static std::string FloatToCXML(const float& f) {
	return "<float> " + std::to_string(f) + "</float>\n";
}

static std::string StringToCXML(const std::string& s) {
	return "<string>" + s + "</string>\n";
}

static std::string IntToCXML(const int& i) {
	return "<int> " + std::to_string(i) + " </int>\n";
}

static std::string Texture2DToCXML(Ref<Texture2D> texture) {
	return "<Texture2D>\n<constructor>\n<string>" + texture->GetName() + "</string>\n<int>" + std::to_string(texture->GetFlags()) + "</int>\n</constructor>\n</Texture2D>\n";
}

static std::string SubTexture2DToCXML(Ref<SubTexture2D> texture) {
	return "<SubTexture2D>\n<constructor>\n" + Texture2DToCXML(texture->GetTexture()) + Vec2fToCXML(texture->m_coords) + Vec2fToCXML(texture->m_cellSize) + Vec2fToCXML(texture->m_spriteSize) + "</constructor>\n</SubTexture2D>\n";
}

static std::string SpriteComponentToCXML(Ref<SpriteComponent> spriteComp) {
	std::string _scene = "<SpriteComponent tag=\"" + spriteComp->m_tag + "\" hash=\"" + std::to_string(spriteComp->hash) + "\">\n";
	std::vector<ShapeID> ids = spriteComp->GetQuads();
	Ref<BatchRenderer> batchRender = spriteComp->GetBatchRenderer();

	for (const ShapeID& id : ids) {
		_scene += "<function>\n";

		if (spriteComp->GetBatchRenderer()->GetShapeSubTexture(id) != nullptr) {
			_scene += "<CreateSubTexturedQuad id=\"" + std::to_string(id) + "\">\n";
			Vector3f p = batchRender->GetShapePosition(id) - spriteComp->GetEntityPosition();
			Vector2f s = batchRender->GetShapeScale(id);
			Vector2f __s = spriteComp->GetEntity()->GetEntityScale().xy();
			s = { s.x / __s.x,s.y / __s.y };
			_scene += Vec3fToCXML(p) + FloatToCXML(batchRender->GetShapeRotation(id)) + Vec3fToCXML({ s,1 }) + Vec4fToCXML(batchRender->GetShapeColor(id)) + SubTexture2DToCXML(batchRender->GetShapeSubTexture(id)) + Vec2fToCXML(batchRender->GetTextureScale(id));
			_scene += "</CreateSubTexturedQuad>\n";
		}
		else {
			_scene += "<CreateQuad id=\"" + std::to_string(id) + "\">\n";
			Vector3f p = batchRender->GetShapePosition(id) - spriteComp->GetEntityPosition();
			Vector2f s = batchRender->GetShapeScale(id);
			Vector2f __s = spriteComp->GetEntity()->GetEntityScale().xy();
			s = { s.x / __s.x,s.y / __s.y };
			_scene += Vec3fToCXML(p) + FloatToCXML(batchRender->GetShapeRotation(id)) + Vec3fToCXML({ s,1 }) + Vec4fToCXML(batchRender->GetShapeColor(id)) + Texture2DToCXML(batchRender->GetShapeTexture(id)) + Vec2fToCXML(batchRender->GetTextureScale(id));
			_scene += "</CreateQuad>\n";
		}

		_scene += "</function>\n";

	}
	_scene += "</SpriteComponent>\n";
	return _scene;
}

static std::string QuadColliderComponentToCXML(Ref<QuadColliderComponent> c) {

	std::string s = "<QuadColliderComponent hash=\"" + std::to_string(c->hash) + "\">\n<constructor>\n";
	int dynamic = c->IsDynamic() ? 1 : 0;
	int physics = c->HasPhysics() ? 1 : 0;
	s += IntToCXML(dynamic) + IntToCXML(physics) + Vec2fToCXML({0,0}) + IntToCXML((int)c->GetFixedRotation()) +std::string("</constructor>\n");

	for (std::pair<const ColliderID, FColliderQuad>& q : c->GetColliders()) {
		s += "<function>\n" + std::string(q.second.quad ? ("<CreateQuad id=\""+std::to_string(q.second.id)+"\">\n") : "<CreateCircle id=\"" + std::to_string(q.second.id) + "\">\n");
		s += Vec2fToCXML(q.second.position) + Vec2fToCXML(q.second.scale) + FloatToCXML(q.second.mass) + FloatToCXML(q.second.rotation) +StringToCXML(q.second.tag)+ FloatToCXML(q.second.bounce) + IntToCXML((int)q.second.quad);
		s += std::string(q.second.quad ? ("</CreateQuad>\n") : "</CreateCircle >\n") + "</function>\n";
	}

	s += "</QuadColliderComponent>\n";
	return s;
}

static std::string CharacterControllerToCXML(Ref<CharacterController> c) {
	return "<CharacterController hash=\"" + std::to_string(c->hash) + "\"></CharacterController>\n";
}

static std::string CharacterGraphicsToCXML(Ref<CharacterGraphics> c) {
	return "<CharacterGraphics hash=\"" + std::to_string(c->hash) + "\"></CharacterGraphics>\n";
}

static std::string AudioListenerComponentToCXML(Ref<AudioListenerComponent> c) {
	return "<AudioListenerComponent hash=\"" + std::to_string(c->hash) + "\"></AudioListenerComponent>\n";
}


static std::string BackgroundEntityToCXML(Ref<BackgroundEntity> e) {
	std::string s = "<BackgroundEntity tag=\"" + e->m_tag + "\" hash=\""+std::to_string(e->hash)+"\">\n";
	const std::unordered_map<std::string, FParalaxBackground>& b = e->GetBackgrounds();
	for (const std::pair<std::string, FParalaxBackground>& p : b) {
		s += "<function>\n<AddParalaxBackground>\n" + StringToCXML(p.first) + Texture2DToCXML(p.second.texture.lock()) + Vec2fToCXML(p.second.scale) + FloatToCXML(p.second.speed) + FloatToCXML(p.second.zOrder) + Vec2fToCXML(p.second.offset);
		s += "</AddParalaxBackground>\n</function>\n";
	}

	s += "</BackgroundEntity>\n";
	return s;
}


static std::string LightComponentToCXML(Ref<LightComponent> lightComp) {
	std::string s = "<LightComponent tag=\"" + lightComp->m_tag + "\" hash=\""+std::to_string(lightComp->hash)+"\">\n";

	int i = 0;
	while (i < 2) {
		Ref<BatchRenderer> batchRender = i == 0 ? lightComp->GetQuadRenderer() : lightComp->GetCircleRenderer();
		const std::vector<ShapeID>& ids = i == 0 ? lightComp->GetQuadLights() : lightComp->GetCircleLights();

		for (const ShapeID& id : ids) {
			s += "<function>\n" + std::string(i == 0 ? "<CreateQuadLight>\n" : "<CreateCircleLight>\n");
			Vector3f p = batchRender->GetShapePosition(id) - lightComp->GetEntityPosition();
			Vector2f sc = batchRender->GetShapeScale(id);
			Vector2f __s = lightComp->GetEntity()->GetEntityScale().xy();
			sc = { sc.x / __s.x,sc.y / __s.y };
			s += Vec3fToCXML(p) + FloatToCXML(batchRender->GetShapeRotation(id)) + Vec3fToCXML({ sc,1 }) + Vec4fToCXML(batchRender->GetShapeColor(id)) + Texture2DToCXML(batchRender->GetShapeTexture(id)) + Vec2fToCXML(batchRender->GetTextureScale(id));
			s += (i == 0 ? "</CreateQuadLight>\n" : "</CreateCircleLight>\n") + std::string("</function>\n");
		}

		i++;
	}

	s += "</LightComponent>\n";
	return s;
}

static std::string TransformToCXML(Ref<Transform> trans) {
	std::string t = "<TransformComponent hash=\""+std::to_string(trans->hash)+"\">\n";
	t += Vec3fToCXML(trans->GetPosition()) + Vec3fToCXML(trans->GetRotation()) + Vec3fToCXML(trans->GetScale());
	t += "</TransformComponent>\n";
	return t;
}


void MapEditor::SaveScene(const std::string& location ) {

	if (Application::DebugTools() && m_debugDrawer) {
		if (m_debugDrawer->GetEntity())
			m_debugDrawer->GetEntity()->Destroy();
		m_debugDrawer = nullptr;
	}

	std::unordered_map<uint64_t, GEngine::Ref<GEngine::Entity>> entities = GEngine::SceneManager::GetCurrentScene()->GetEntities();
	std::string _scene;
	std::vector<uint64_t> sortedHash;
	for (const std::pair<uint64_t, Ref<Entity>>& e : entities) {
		sortedHash.push_back(e.first);
	}
	std::sort(sortedHash.begin(), sortedHash.end());


	for (const uint64_t& hash: sortedHash) {
		Ref<Entity> e = entities[hash];

		if (auto _c = dynamic_pointer_cast<CharacterEntity>(e)) {
			_scene += "<CharacterEntity tag=\"" + e->m_tag + "\" hash=\""+std::to_string(e->hash)+"\">\n";
			_scene += TransformToCXML(_c->GetEntityTransformComponent());
			_scene += "</CharacterEntity>\n";
			continue;
		}
		else if (auto _c = dynamic_pointer_cast<BackgroundEntity>(e)) {
			_scene += BackgroundEntityToCXML(_c);
			continue;
		}
		else {
			std::string s = typeid(*e.get()).name();
			if (s.find("GEngine::Entity") != std::string::npos || s.find("GEngine6Entity") != std::string::npos ) {
				_scene += "<Entity tag=\"" + e->m_tag + "\" hash=\""+std::to_string(hash)+"\">\n";
			}
			else {
				#ifdef GE_PLATFORM_WINDOWS
				s.erase(s.find("class "), 6);
				#else
				int index = 0;
				if (s[0] == 'N') {
					int counter = 0;
					index++;
					while (isdigit(s[index]) ) { index++; counter++;};
					int nOffset = stoi(s.substr(1, counter));
					index = 1+nOffset+counter;
				}
				int nIndex = index;
				while (isdigit(s[nIndex]) ) { nIndex++;};
				int nOffset = stoi(s.substr(index, nIndex-index));
				s = s.substr(nIndex, nOffset);
				#endif
				_scene += "<" + s + " tag=\"" + e->m_tag + "\" hash=\"" + std::to_string(e->hash) + "\">\n";
				_scene += Vec3fToCXML(e->GetEntityPosition()) + Vec3fToCXML(e->GetEntityRotation()) + Vec3fToCXML(e->GetEntityScale());
				_scene += "</" + s + ">\n";
				continue;
			}
			
			
		}

		for (const std::pair<uint64_t, Ref<Component>>& c : e->GetComponents()) {
			if (auto _c = dynamic_pointer_cast<CharacterController>(c.second)) {
				_scene += CharacterControllerToCXML(_c);
				continue;
			}
			if (auto _c = dynamic_pointer_cast<CharacterGraphics>(c.second)) {
				_scene += CharacterGraphicsToCXML(_c);
				continue;
			}
			if (auto _c = dynamic_pointer_cast<AudioListenerComponent>(c.second)) {
				_scene += AudioListenerComponentToCXML(_c);
				continue;
			}
			if (auto _c = dynamic_pointer_cast<Transform>(c.second)) {
				_scene += TransformToCXML(_c);
				continue;
			}
			if (auto _c = dynamic_pointer_cast<SpriteComponent>(c.second)) {
				_scene += SpriteComponentToCXML(_c);
				continue;
			}
			if (auto _c = dynamic_pointer_cast<LightComponent>(c.second)) {
				_scene += LightComponentToCXML(_c);
				continue;
			}
			if (auto _c = dynamic_pointer_cast<QuadColliderComponent>(c.second)) {
				_scene += QuadColliderComponentToCXML(_c);
				continue;
			}
		}
		_scene += "</Entity>\n";
	}

	std::ofstream _outFile(FileSystem::GetParentExecuteableDir(3) + "WizardJump/"+ location);
	_outFile.write(_scene.c_str(), _scene.size());
	_outFile.close();

	
	bool b = FileSystem::FileInMemory(location);
	if (b) {
		Ref<FileData> data = FileSystem::FileDataFromPath(location);
		data->Clear();
		void* _scData = malloc(_scene.size() * sizeof(char));
		memcpy(_scData, &_scene[0], _scene.size() * sizeof(char));
		data->SetData(_scene.size() * sizeof(char), (unsigned char*)_scData);
	}
	else {
		void* _scData = malloc(_scene.size() * sizeof(char));
		memcpy(_scData, &_scene[0], _scene.size() * sizeof(char));
		Ref<FileData> data = make_shared<FileData>(_scene.size() * sizeof(char), (unsigned char*)_scData);
		FileSystem::AddToMemoryPak(location, data);
	}

	Ref<Entity> e = CreateGameObject<Entity>();
	e->m_tag = "__EditorEntity";
	m_debugDrawer = CreateGameObject<SpriteComponent>();
	AddEntity(e);
	e->AddComponent(m_debugDrawer);

}

static Ref<Texture2D> GetNodeAsTexture2D(const CXML::CXML_Node& n, size_t* endPos = nullptr) {
	size_t _endPos;
	CXML::CXML_Node cons = cxml.GetNext(n.info.c_str());
	std::string name = cxml.GetNextAsString(cons.info.c_str(), 0, &_endPos);
	int flag = cxml.GetNextAsInt(cons.info.c_str(), _endPos);
	if (endPos)
		*endPos = n.endPos;
	return Texture2D::Create(name, flag);
}

static Ref<SubTexture2D> GetNodeAsSubTexture2D(const CXML::CXML_Node& n, size_t* endPos) {
	size_t _endPos;
	CXML::CXML_Node cons = cxml.GetNext(n.info.c_str());
	Ref<Texture2D> texture = GetNodeAsTexture2D(cxml.GetNext(cons.info.c_str()), &_endPos);
	Vector2f coords = cxml.GetNextAsFloats(cons.info.c_str(), 2, cxml.GetNext(cons.info.c_str()).endPos, &_endPos).data();
	Vector2f cellSize = cxml.GetNextAsFloats(cons.info.c_str(), 2, _endPos, &_endPos).data();
	Vector2f spriteSize = cxml.GetNextAsFloats(cons.info.c_str(), 2, _endPos, &_endPos).data();
	if (endPos)
		*endPos = n.endPos;
	return SubTexture2D::CreateFromCoords(texture, coords, cellSize, spriteSize);
}

static Ref<SpriteComponent> GetNodeAsSpriteComponent(Ref<Entity> e, const CXML::CXML_Node& n, size_t* _endPos = nullptr) {
	size_t endPos;
	CXML::CXML_Node _n = cxml.GetNext(n.info.c_str());
	Ref<SpriteComponent> sprEnt = CreateGameObject_ID<SpriteComponent>(std::stoull(n.tags.at("hash")));
	e->AddComponent(sprEnt);
	while (CXML::ValidNode(_n)) {
		if (_n.type == "function") {
			if (sprEnt != nullptr) {
				CXML::CXML_Node __n = cxml.GetNext(_n.info.c_str());
				if (__n.type == "CreateQuad") {
					Vector3f pos = cxml.GetNextAsFloats(__n.info.c_str(), 3, 0, &endPos).data();
					float rot = cxml.GetNextAsFloat(__n.info.c_str(), endPos, &endPos);
					Vector3f scale = cxml.GetNextAsFloats(__n.info.c_str(), 3, endPos, &endPos).data();
					Vector4f color = cxml.GetNextAsFloats(__n.info.c_str(), 4, endPos, &endPos).data();
					Ref<Texture2D> texture = GetNodeAsTexture2D(cxml.GetNext(__n.info.c_str(), endPos), &endPos);
					Vector2f textureScale = cxml.GetNextAsFloats(__n.info.c_str(), 2, endPos, &endPos).data();
					sprEnt->CreateQuad(pos, rot, scale, color, texture, textureScale);
				}
				else if (__n.type == "CreateSubTexturedQuad") {
					Vector3f pos = cxml.GetNextAsFloats(__n.info.c_str(), 3, 0, &endPos).data();
					float rot = cxml.GetNextAsFloat(__n.info.c_str(), endPos, &endPos);
					Vector3f scale = cxml.GetNextAsFloats(__n.info.c_str(), 3, endPos, &endPos).data();
					Vector4f color = cxml.GetNextAsFloats(__n.info.c_str(), 4, endPos, &endPos).data();
					Ref<SubTexture2D> texture = GetNodeAsSubTexture2D(cxml.GetNext(__n.info.c_str(), endPos), &endPos);
					Vector2f textureScale = cxml.GetNextAsFloats(__n.info.c_str(), 2, endPos, &endPos).data();
					sprEnt->CreateSubTexturedQuad(pos, rot, scale, color, texture, textureScale);
				}
			}
		}
		_n = cxml.GetNext(n.info.c_str(), _n.endPos);
	}
	if (_endPos)
		*_endPos = n.endPos;
	return sprEnt;
}

static Ref<LightComponent> GetNodeAsLightComponent(Ref<Entity> e, const CXML::CXML_Node& n, size_t* _endPos = nullptr) {
	size_t endPos;
	CXML::CXML_Node _n = cxml.GetNext(n.info.c_str());
	Ref<LightComponent> sprEnt = CreateGameObject_ID<LightComponent>(std::stoull(n.tags.at("hash")));
	e->AddComponent(sprEnt);
	while (CXML::ValidNode(_n)) {
		if (_n.type == "function") {
			if (sprEnt != nullptr) {
				CXML::CXML_Node __n = cxml.GetNext(_n.info.c_str());
				if (__n.type == "CreateCircleLight" || "CreateQuadLight") {
					Vector3f pos = cxml.GetNextAsFloats(__n.info.c_str(), 3, 0, &endPos).data();
					float rot = cxml.GetNextAsFloat(__n.info.c_str(), endPos, &endPos);
					Vector3f scale = cxml.GetNextAsFloats(__n.info.c_str(), 3, endPos, &endPos).data();
					Vector4f color = cxml.GetNextAsFloats(__n.info.c_str(), 4, endPos, &endPos).data();
					Ref<Texture2D> texture = GetNodeAsTexture2D(cxml.GetNext(__n.info.c_str(), endPos), &endPos);
					Vector2f textureScale = cxml.GetNextAsFloats(__n.info.c_str(), 2, endPos, &endPos).data();
					__n.type == "CreateCircleLight" ? sprEnt->AddCircleLight(pos, rot, scale, color) : sprEnt->AddQuadLight(pos, rot, scale, color, texture);
				}
			}
		}
		_n = cxml.GetNext(n.info.c_str(), _n.endPos);
	}
	if (_endPos)
		*_endPos = n.endPos;

	return sprEnt;
}

static Ref<CharacterController> GetNodeAsCharacterController(Ref<Entity> e, const CXML::CXML_Node& n, size_t* endPos = nullptr) {
	Ref<CharacterController> c = CreateGameObject_ID<CharacterController>(std::stoull(n.tags.at("hash")));
	e->AddComponent(c);
	if(endPos)
		*endPos = n.endPos;
	return c;
}

static Ref<CharacterGraphics> GetNodeAsCharacterGraphics(Ref<Entity> e, const CXML::CXML_Node& n, size_t* endPos = nullptr) {
	Ref<CharacterGraphics> c = CreateGameObject_ID<CharacterGraphics>(std::stoull(n.tags.at("hash")));
	e->AddComponent(c);
	if (endPos)
		*endPos = n.endPos;
	return c;
}

static Ref<AudioListenerComponent> GetNodeAsAudioListener(Ref<Entity> e, const CXML::CXML_Node& n, size_t* endPos = nullptr) {
	Ref<AudioListenerComponent> c = CreateGameObject_ID<AudioListenerComponent>(std::stoull(n.tags.at("hash")));
	e->AddComponent(c);
	if (endPos)
		*endPos = n.endPos;
	return c;
}

static Ref<BackgroundEntity> GetNodeAsBackgroundEntity(Scene* s, const CXML::CXML_Node& n, size_t* _endPos = nullptr) {

	Ref<BackgroundEntity> e = CreateGameObject_ID<BackgroundEntity>(std::stoull(n.tags.at("hash")));
	e->m_tag = n.tags.at("tag");
	s->AddEntity(e);

	CXML::CXML_Node _n = cxml.GetNext(n.info.c_str());

	while (CXML::ValidNode(_n)) {

		if (_n.type == "function") {
			CXML::CXML_Node __n = cxml.GetNext(_n.info.c_str());
			if (__n.type == "AddParalaxBackground") {
				size_t endPos = 0;
				std::string s = cxml.GetNextAsString(__n.info.c_str(), endPos, &endPos);
				CXML::CXML_Node _tex = cxml.GetNext(__n.info.c_str(), endPos);
				endPos = _tex.endPos;
				Ref<Texture2D> tex = GetNodeAsTexture2D(_tex);
				Vector2f sc = cxml.GetNextAsFloats(__n.info.c_str(), 2, endPos, &endPos).data();
				float sp = cxml.GetNextAsFloat(__n.info.c_str(), endPos, &endPos);
				float order = cxml.GetNextAsFloat(__n.info.c_str(), endPos, &endPos);
				Vector2f offset = cxml.GetNextAsFloats(__n.info.c_str(), 2, endPos, &endPos).data();

				e->AddParalaxBackground(s, tex, sc, sp, order, offset);
			}
		}

		_n = cxml.GetNext(n.info.c_str(), _n);
	}

	if (_endPos)
		*_endPos = n.endPos;

	return e;
}

static Ref<QuadColliderComponent> GetNodeAsQuadColliderComponent(Ref<Entity> e, const CXML::CXML_Node& n, size_t* _endPos){
	Ref<QuadColliderComponent> c;

	CXML::CXML_Node _n = cxml.GetNext(n.info.c_str());

	while (CXML::ValidNode(_n)) {
		if (_n.type == "constructor") {
			size_t pos = 0;
			Vector2<int> cc = cxml.GetNextAsInts(_n.info.c_str(), 2, pos, &pos).data();
			Vector2f poss = cxml.GetNextAsFloats(_n.info.c_str(), 2, pos, &pos).data();
			int rot = cxml.GetNextAsInt(_n.info.c_str(), pos, &pos);
			c = CreateGameObject_ID<QuadColliderComponent>(std::stoull(n.tags.at("hash")), (bool)cc.x, (bool)cc.y, poss, (bool)rot);
			e->AddComponent(c);
		}
		if (_n.type == "function") {
			CXML::CXML_Node __n = cxml.GetNext(_n.info.c_str());

			if (__n.type == "CreateQuad" || __n.type == "CreateCircle") {
				size_t pos = 0;
				Vector2f poss = cxml.GetNextAsFloats(__n.info.c_str(), 2, pos, &pos).data();
				Vector2f scale = cxml.GetNextAsFloats(__n.info.c_str(), 2, pos, &pos).data();
				Vector2f mssrot = cxml.GetNextAsFloats(__n.info.c_str(), 2, pos, &pos).data();
				std::string t = cxml.GetNextAsString(__n.info.c_str(), pos, &pos);
				float bounce = cxml.GetNextAsFloat(__n.info.c_str(), pos, &pos);
				ColliderID id = __n.type == "CreateQuad" ? c->CreateQuad(poss, scale, mssrot.x, mssrot.y, t) : c->CreateCircle(poss, scale, mssrot.x, mssrot.y, t);
				c->SetBounce(id, bounce);
				ThreadPool::AddMainThreadFunction([c]() {c->WakeBody(); });
			}
		}
		_n = cxml.GetNext(n.info.c_str(), _n.endPos);
	}

	return c;
}

void MapEditor::LoadScene(const std::string& scene)
{
#ifdef GE_CONSOLE_APP
	hashSelected = nullptr;
	compSelected = nullptr;
	intHashSelected = 0;
	b_component = false;
	m_debugDrawer = nullptr;
	characterEntity = nullptr;
	entities.clear();
	Ref<Entity> e = CreateGameObject<Entity>();
	e->m_tag = "__EditorEntity";
	m_debugDrawer = CreateGameObject<SpriteComponent>();
	AddEntity(e);
	e->AddComponent(m_debugDrawer);

#endif

	Ref<Entity> lastEntity = nullptr;
	CXML::CXML_Node n = cxml.GetNext(scene.c_str());
	while (CXML::ValidNode(n)) {
		if (n.type == "Entity") {
			Ref<Entity> e = CreateGameObject_ID<Entity>(std::stoull(n.tags.at("hash")));
			e->m_tag = n.tags["tag"];
			AddEntity(e);
			CXML::CXML_Node c = cxml.GetNext(n.info.c_str());
			size_t endPos;
			while (CXML::ValidNode(c)) {

				if (c.type == "TransformComponent") {
					Vector3f pos = cxml.GetNextAsFloats(c.info.c_str(), 3, 0, &endPos).data();
					Vector3f rot = cxml.GetNextAsFloats(c.info.c_str(), 3, endPos, &endPos).data();
					Vector3f scale = cxml.GetNextAsFloats(c.info.c_str(), 3, endPos, &endPos).data();

					e->SetEntityPosition(pos);
					e->SetEntityRotation(rot);
					e->SetEntityScale(scale);
				} else if (c.type == "SpriteComponent") {
					GetNodeAsSpriteComponent(e, c);
				} else if (c.type == "LightComponent") {
					GetNodeAsLightComponent(e, c);
				} else if (c.type == "QuadColliderComponent") {
					GetNodeAsQuadColliderComponent(e, c);
				} else if (c.type == "AudioListenerComponent") {
					GetNodeAsAudioListener(e, c);
				}
				
				c = cxml.GetNext(n.info.c_str(), c);
			}

		}
		else if (n.type == "CharacterEntity") {

			CXML::CXML_Node c = cxml.GetNext(n.info.c_str());
			size_t endPos;
			while (CXML::ValidNode(c)) {
				if (c.type == "TransformComponent") {
					Vector3f pos = cxml.GetNextAsFloats(c.info.c_str(), 3, 0, &endPos).data();
					Vector3f rot = cxml.GetNextAsFloats(c.info.c_str(), 3, endPos, &endPos).data();
					Vector3f scale = cxml.GetNextAsFloats(c.info.c_str(), 3, endPos, &endPos).data();
					Ref<Entity> e = dynamic_pointer_cast<Entity>(entityMap[n.type](std::stoull(n.tags.at("hash"))));
					e->m_tag = n.tags["tag"];
					e->SetEntityPosition(pos);
					e->SetEntityRotation(rot);
					e->SetEntityScale(scale);
					c = cxml.GetNext(n.info.c_str(), c);
				}
				
			}
		}
		else if (n.type == "BackgroundEntity") {
			GetNodeAsBackgroundEntity(this, n);
		}
		else {
			if (entityMap.find(n.type) != entityMap.end()) {
				CXML::CXML_Node c = n;
				size_t endPos = 0;
				Ref<Entity> e = dynamic_pointer_cast<Entity>(entityMap[n.type](std::stoull(n.tags.at("hash"))));
				e->m_tag = n.tags["tag"];
				Vector3f pos = cxml.GetNextAsFloats(c.info.c_str(), 3, 0, &endPos).data();
				Vector3f rot = cxml.GetNextAsFloats(c.info.c_str(), 3, endPos, &endPos).data();
				Vector3f scale = cxml.GetNextAsFloats(c.info.c_str(), 3, endPos, &endPos).data();
				e->SetEntityPosition(pos);
				e->SetEntityRotation(rot);
				e->SetEntityScale(scale);

			}
		}
		n = cxml.GetNext(scene.c_str(), n.endPos);
	}
	
}
