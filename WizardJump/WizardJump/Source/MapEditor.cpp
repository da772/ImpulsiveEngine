#include "MapEditor.hpp"
#include "DebugLayer.h"
#include "Environment/SpriteEntity.hpp"
#include "CXML/CXML.hpp"


static CXML cxml;

MapEditor::MapEditor(const char* id, Camera* camera) : Scene(id, camera)
{
	
}

MapEditor::~MapEditor()
{

}

void MapEditor::OnEvent(Event& e)
{
	m_CameraController->OnEvent(e);
}

void MapEditor::OnUpdate(Timestep timestep)
{
	m_CameraController->OnUpdate(timestep);
}

void MapEditor::OnBegin()
{
	DebugLayer::showScene = false;
	camera = m_CameraController->GetCamera().get();
	GEngine::Application::GetApp()->SetTargetCamera(camera);

	return;
	Ref<SpriteEntity> spEnt = CreateGameObject<SpriteEntity>(Vector3f(0, 0, 0), Vector3f(1, 1, 1), 0.f);
	AddEntity(spEnt);
	spEnt->GetSpriteComponent()->CreateSubTexturedQuad({ 0,0,0 }, 0, { 1,1,1 }, { 1,1,1,1 }, SubTexture2D::CreateFromCoords(Texture2D::Create("Content/Textures/wiz10_face.png", TEXTUREFLAGS_Wrap_ClampToEdge), { 0,0 }, { 128,128 }));

	spEnt->GetSpriteComponent()->CreateSubTexturedQuad({ 0,0,0 }, 0, { 1,1,1 }, { 1,1,1,1 }, SubTexture2D::CreateFromCoords(Texture2D::Create("Content/Textures/towerUnder.png", TEXTUREFLAGS_Wrap_ClampToEdge), { 0,0 }, { 128,128 }));


	spEnt->GetSpriteComponent()->CreateQuad({ 0,0,-5 }, 0, { 200,200,1 }, { 1,1,1,1 }, Texture2D::Create("Content/Textures/Checkerboard.png", TEXTUREFLAGS_Wrap_Repeat), { 200,200 });
};


void MapEditor::OnEnd()
{

}

void MapEditor::OnImGuiRender()
{
	SceneMenu();
}

void MapEditor::OnLoad()
{
	SetupCamera();
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
			m_CameraController->OnCameraZoom(e.GetXOffset(), e.GetYOffset(), .1f);
			return false;
			});
		});
}


static GEngine::Ref<GEngine::GameObject> hashSelected = nullptr;
static bool b_component = false;

void MapEditor::SceneMenu()
{

	

	ImGui::Begin("Scene Hierarchy"); {


		if (ImGui::Button("Add")) {
			if (dynamic_pointer_cast<Component>(hashSelected) != nullptr) {
				if (dynamic_pointer_cast<SpriteComponent>(hashSelected)) {
					Ref<SpriteComponent> s = dynamic_pointer_cast<SpriteComponent>(hashSelected);
					s->CreateQuad({ 0,0, 0 });
				}
			}
		}



		if (hashSelected != nullptr) {
			ImGui::SameLine();
			if (ImGui::Button("Destroy")) {

				if (b_component) {
					GEngine::Ref<GEngine::Component> __c = static_pointer_cast<GEngine::Component>(hashSelected);
					__c->Destroy();
				}
				else {
					GEngine::Ref<GEngine::Entity> __e = static_pointer_cast<GEngine::Entity>(hashSelected);
					__e->Destroy();
				}

				hashSelected = nullptr;
				b_component = false;

			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Save")) {
			SaveScene();
		}

		ImGui::SameLine();
		if (ImGui::Button("Load")) {
			LoadScene((char*)FileSystem::FileDataFromPath("Content/Scenes/Scene.scene")->GetDataAsString());
		}

		ImGui::Separator();

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::TreeNode("Entities"))
		{
			std::unordered_map<uint64_t, GEngine::Ref<GEngine::Entity>> entities = GEngine::SceneManager::GetCurrentScene()->GetEntities();
			for (const std::pair<uint64_t, GEngine::Ref<GEngine::Entity>>& e : entities)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 1, 0, 1));
				ImGuiTreeNodeFlags entity_base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
				if (hashSelected == e.second) {
					entity_base_flags |= ImGuiTreeNodeFlags_Selected;
				}
				bool show_e = ImGui::TreeNodeEx((void*)(intptr_t)e.first, entity_base_flags, "%s : %s (%ju)", e.second->m_tag.c_str(), typeid(*e.second.get()).name(), e.first);
				if (ImGui::IsItemClicked()) {
					b_component = false;
					hashSelected = e.second;
				}

				if (show_e)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.75, .75, .75, 1));
					std::unordered_map<uint64_t, GEngine::Ref<GEngine::Component>> components = e.second->GetComponents();


					for (const std::pair<uint64_t, GEngine::Ref<GEngine::Component>>& c : components) {
						std::string compName = typeid(*c.second.get()).name();
						ImGuiTreeNodeFlags component_base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
						if (hashSelected == c.second) {
							component_base_flags |= ImGuiTreeNodeFlags_Selected;
						}
						bool show_c = ImGui::TreeNodeEx((void*)(intptr_t)c.first, component_base_flags, "%s : %s (%ju)", c.second->m_tag.c_str(), typeid(*c.second.get()).name(), c.first);
						if (ImGui::IsItemClicked()) {
							b_component = true;
							hashSelected = c.second;
						}
						if (show_c) {

							if (compName == "class GEngine::Transform") {
								float v[3];
								GEngine::Ref<GEngine::Transform> transform = dynamic_pointer_cast<GEngine::Transform>(c.second);

								if (!transform) return;

								v[0] = transform->GetPosition().x;
								v[1] = transform->GetPosition().y;
								v[2] = transform->GetPosition().z;
								ImGui::Text("Position: ");
								ImGui::SameLine();
								if (ImGui::InputFloat3("tPos", v, 4)) {
									if (v[0] != transform->GetPosition().x || v[1] != transform->GetPosition().y || v[2] != transform->GetPosition().z) {
										e.second->SetEntityPosition({ v[0], v[1], v[2] });
									}
								}
								float r[3];
								memcpy(r, e.second->GetEntityRotation().data(), sizeof(float) * 3);
								ImGui::Text("Rotation: ");
								ImGui::SameLine();
								if (ImGui::InputFloat3("eRot", r)) {
									if (transform->GetRotation() != Vector3f(r)) {
										e.second->SetEntityRotation(r);
									}
								}

								float s[3];
								ImGui::Text("Scale: ");
								ImGui::SameLine();
								Vector3f _s = transform->GetScale();
								memcpy(s, _s.data(), sizeof(float) * 3);
								if (ImGui::InputFloat3("tScale", s)) {
									if (_s != Vector3f(s)) {
										e.second->SetEntityScale(Vector3f(s));
									}
								}

							}
							if (compName == "class GEngine::SpriteComponent") {
								GEngine::Ref<GEngine::SpriteComponent> spriteComp = dynamic_pointer_cast<GEngine::SpriteComponent>(c.second);
								std::vector<ShapeID> ids = spriteComp->GetQuads();
								Ref<BatchRenderer> batchRender = spriteComp->GetBatchRenderer();
								int counter = 0;
								for (const ShapeID& id : ids) {
									bool show_sid = ImGui::TreeNodeEx((void*)(intptr_t)ids[counter++], component_base_flags, "%llu", id);
									if (show_sid) {
										Vector3f p = batchRender->GetShapePosition(id)-e.second->GetEntityPosition();
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
										float rot = _rot-e.second->GetEntityRotation().z;
										ImGui::Text("Rotation: ");
										ImGui::SameLine();
										ImGui::InputFloat("rot", &rot);
										if (rot != _rot-e.second->GetEntityRotation().z) {
											spriteComp->SetRotation(id, rot);
										}
										float _s[2];
										ImGui::Text("Scale: ");
										ImGui::SameLine();
										Vector2f s = batchRender->GetShapeScale(id);
										Vector2f __s = e.second->GetEntityScale().xy();
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

										if (batchRender->GetShapeSubTexture(id)) {
											uint64_t __id = id + 1;
											bool show_subTId = ImGui::TreeNodeEx((void*)(intptr_t)__id, component_base_flags, "Sub Texture");
											if (show_subTId) {
												bool change = false;
												Ref<SubTexture2D> subT = batchRender->GetShapeSubTexture(id);
												std::string tex = subT->GetTexture()->GetName();
												char ch[512] = { 0 };
												memcpy(ch, &tex[0], tex.size() * sizeof(char));
												if (ImGui::InputText("texStr", ch, 255, ImGuiInputTextFlags_EnterReturnsTrue)) {
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
													subT->SetCoords(*subT->GetTexCoords(), subSiz);
													change = true;
												}

												float spSize[2];
												memcpy(spSize, subT->m_spriteSize.data(), sizeof(float)*2);
												ImGui::Text("Sprite Size: ");
												ImGui::SameLine();
												if (ImGui::InputFloat2("sizeCell", spSize)) {
													subT->SetCoords(*subT->GetTexCoords(), subT->m_cellSize, spSize);
													change = true;
												}

												if (change) {
													subT = SubTexture2D::CreateFromCoords(Texture2D::Create(std::string(ch), TEXTUREFLAGS_Min_Nearest | TEXTUREFLAGS_Mag_Nearest),
														*subT->GetTexCoords(), subT->m_cellSize, subT->m_spriteSize);
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

					ImGui::TreePop();
				}
				ImGui::PopStyleColor(1);
			}

			ImGui::TreePop();
		}


	}
	ImGui::End();
}


static std::string Vec3fToCXML(const Vector3f& v) {
	return "<float> " + std::to_string(v.x) + "</float>\n" + "<float> " + std::to_string(v.y) + " </float>\n" + "<float> " + std::to_string(v.z) + " </float>\n";
}
static std::string Vec4fToCXML(const Vector4f& v) {
	return "<float> " + std::to_string(v.x) + "</float>\n" + "<float> " + std::to_string(v.y) + " </float>\n" + "<float> " + std::to_string(v.z) + " </float>\n" + "<float> " + std::to_string(v.w) + " </float>\n";
}
static std::string Vec2fToCXML(const Vector2f& v) {
	return "<float> " + std::to_string(v.x) + "</float>\n" + "<float> " + std::to_string(v.y) + " </float>\n";
}

static std::string FloatToCXML(const float& f) {
	return "<float> " + std::to_string(f) + "</float>\n";
}

static std::string StringToCXML(const std::string& s) {
	return "<string>" + s + "</string>\n";
}

static std::string Texture2DToCXML(Ref<Texture2D> texture) {
	return "<Texture2D>\n<constructor>\n<string>"+texture->GetName()+"</string>\n<int>"+std::to_string(texture->GetFlags())+"</int>\n</constructor>\n</Texture2D>\n";
}

static std::string SubTexture2DToCXML(Ref<SubTexture2D> texture) {
	return "<SubTexture2D>\n<constructor>\n" + Texture2DToCXML(texture->GetTexture()) + Vec2fToCXML(*texture->GetTexCoords()) + Vec2fToCXML(texture->m_cellSize) + Vec2fToCXML(texture->m_spriteSize) + "</constructor>\n</SubTexture2D>\n";
}


void MapEditor::SaveScene() {

	std::unordered_map<uint64_t, GEngine::Ref<GEngine::Entity>> entities = GEngine::SceneManager::GetCurrentScene()->GetEntities();

	std::string _scene;


	for (const std::pair<uint64_t, Ref<Entity>>& e : entities) {

		if (auto s = dynamic_pointer_cast<SpriteEntity>(e.second)) {

			_scene += "<SpriteEntity tag=" + e.second->m_tag + ">\n<constructor>\n" + Vec3fToCXML(e.second->GetEntityPosition()) + Vec2fToCXML(e.second->GetEntityScale().xy()) + FloatToCXML(e.second->GetEntityRotation().z) + "\n</constructor>\n";

			std::vector<ShapeID> ids = s->GetSpriteComponent()->GetQuads();

			Ref<BatchRenderer> batchRender =  s->GetSpriteComponent()->GetBatchRenderer();

			for (const ShapeID& id : ids) {
				_scene += "<function>\n";

				if (s->GetSpriteComponent()->GetBatchRenderer()->GetShapeSubTexture(id) != nullptr) {
					_scene += "<CreateSubTexturedQuad id=" + std::to_string(id) + ">\n";
					Vector3f p = batchRender->GetShapePosition(id) - e.second->GetEntityPosition();
					Vector2f s = batchRender->GetShapeScale(id);
					Vector2f __s = e.second->GetEntityScale().xy();
					s = { s.x / __s.x,s.y / __s.y };
					_scene += Vec3fToCXML(p) + FloatToCXML(batchRender->GetShapeRotation(id)) + Vec3fToCXML({ s,1 }) + Vec4fToCXML(batchRender->GetShapeColor(id))+SubTexture2DToCXML(batchRender->GetShapeSubTexture(id)) + Vec2fToCXML(batchRender->GetTextureScale(id));
					_scene += "</CreateSubTexturedQuad>\n";
				}
				else {
					_scene += "<CreateQuad id="+std::to_string(id)+">\n";
					Vector3f p = batchRender->GetShapePosition(id) - e.second->GetEntityPosition();
					Vector2f s = batchRender->GetShapeScale(id);
					Vector2f __s = e.second->GetEntityScale().xy();
					s = { s.x / __s.x,s.y / __s.y };
					_scene += Vec3fToCXML(p) + FloatToCXML(batchRender->GetShapeRotation(id)) + Vec3fToCXML({ s,1 }) + Vec4fToCXML(batchRender->GetShapeColor(id))+Texture2DToCXML(batchRender->GetShapeTexture(id))+Vec2fToCXML(batchRender->GetTextureScale(id));
					_scene += "</CreateQuad>\n";
				}

				_scene += "</function>\n";

			}
			_scene += "</SpriteEntity>\n";
		}


	}

	std::ofstream _outFile(FileSystem::GetParentExecuteableDir(3) + "WizardJump/Content/Scenes/Scene.scene");
	_outFile.write(_scene.c_str(), _scene.size());
	_outFile.close();

	Ref<FileData> data = FileSystem::FileDataFromPath("Content/Scenes/Scene.scene");
	data->Clear();
	void* _scData = malloc(_scene.size() * sizeof(char));
	memcpy(_scData, &_scene[0], _scene.size() * sizeof(char));
	data->SetData(_scene.size() * sizeof(char), (unsigned char*)_scData);
	
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

void MapEditor::LoadScene(const std::string& scene)
{
	Ref<Entity> lastEntity = nullptr;
	CXML::CXML_Node n = cxml.GetNext(scene.c_str());
	while (CXML::ValidNode(n)) {

		if (n.type == "SpriteEntity") {
			size_t endPos;
			CXML::CXML_Node _n = cxml.GetNext(n.info.c_str());
			while (CXML::ValidNode(_n)) {
				if (_n.type == "constructor") {
					Vector3f pos = cxml.GetNextAsFloats(_n.info.c_str(), 3, 0, &endPos).data();
					Vector2f scale = cxml.GetNextAsFloats(_n.info.c_str(), 2, endPos, &endPos).data();
					float rot = cxml.GetNextAsFloat(_n.info.c_str(), endPos, &endPos);
					lastEntity = CreateGameObject<SpriteEntity>(pos, scale, rot);
					AddEntity(lastEntity);
				}
				if (_n.type == "function") {
					Ref<SpriteEntity> sprEnt = dynamic_pointer_cast<SpriteEntity>(lastEntity);
					if (sprEnt != nullptr) {
						CXML::CXML_Node __n = cxml.GetNext(_n.info.c_str());
						if (__n.type == "CreateQuad") {
							Vector3f pos = cxml.GetNextAsFloats(__n.info.c_str(), 3, 0, &endPos).data();
							float rot = cxml.GetNextAsFloat(__n.info.c_str(), endPos, &endPos);
							Vector3f scale = cxml.GetNextAsFloats(__n.info.c_str(), 3, endPos, &endPos).data();
							Vector4f color = cxml.GetNextAsFloats(__n.info.c_str(), 4, endPos, &endPos).data();
							Ref<Texture2D> texture = GetNodeAsTexture2D(cxml.GetNext(__n.info.c_str(), endPos), &endPos);
							Vector2f textureScale = cxml.GetNextAsFloats(__n.info.c_str(), 2, endPos, &endPos).data();
							sprEnt->GetSpriteComponent()->CreateQuad(pos, rot, scale, color, texture, textureScale);
						}
						else if (__n.type == "CreateSubTexturedQuad") {
							Vector3f pos = cxml.GetNextAsFloats(__n.info.c_str(), 3, 0, &endPos).data();
							float rot = cxml.GetNextAsFloat(__n.info.c_str(), endPos, &endPos);
							Vector3f scale = cxml.GetNextAsFloats(__n.info.c_str(), 3, endPos, &endPos).data();
							Vector4f color = cxml.GetNextAsFloats(__n.info.c_str(), 4, endPos, &endPos).data();
							Ref<SubTexture2D> texture = GetNodeAsSubTexture2D(cxml.GetNext(__n.info.c_str(), endPos), &endPos);
							Vector2f textureScale = cxml.GetNextAsFloats(__n.info.c_str(), 2, endPos, &endPos).data();
							sprEnt->GetSpriteComponent()->CreateSubTexturedQuad(pos, rot, scale, color, texture, textureScale);
						}
					}
					
				}
				_n = cxml.GetNext(n.info.c_str(), _n.endPos);
			}



		}

		n = cxml.GetNext(scene.c_str(), n.endPos);
	}
	
}
