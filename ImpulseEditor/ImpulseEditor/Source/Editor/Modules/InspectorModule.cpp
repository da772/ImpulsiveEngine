#ifdef GE_EDITOR
#include "InspectorModule.h"
#include "Shared/ImpulseEditor.h"
#include "Editor/Modules/ReloadModule.h"
#include "imgui/imgui_internal.h"
#include "Editor/EditorLayer.h"
#include "Editor/Events/EditorSceneEvents.h"


namespace Editor {

	static std::set<std::string> EngineComponents = {"Transform", "RuntimeMeshComponent", "SpriteComponent", "UIComponent", "ButtonComponent", "QuadColliderComponent", "SpriteAnimationComponent", "AudioComponent",
	"AudioListenerComponent"};
	
	InspectorModule::InspectorModule(std::set<GEngine::ObjectHash>* selectedGameObject, ReloadModule* reloadModule) : m_selectedGameObject(selectedGameObject), reloadModule(reloadModule)
	{
		s_ComponentMap = 
		{
			{ "Transform Component", BIND_EVENT_FN(InspectorModule, Inspect_TransformComponent)},
			{"Native Script Component", BIND_EVENT_FN(InspectorModule, Inspect_NativeComponent)},
			{"Sprite Component",BIND_EVENT_FN(InspectorModule, Inspect_SpriteComponent)},
			{"UI Component",BIND_EVENT_FN(InspectorModule, Inspect_UIComponent)}
		};
		m_textures["remove"] = GEngine::Texture2D::Create("Content/EditorContent/Textures/Icons/delete192x129.png");
	}

	InspectorModule::~InspectorModule()
	{

	}

	void InspectorModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{
		if (!ImGui::Begin(name.c_str(), is_open, flags | ImGuiWindowFlags_HorizontalScrollbar)) {
			ImGui::End();
			return;
		}
		if (m_selectedGameObject->size() == 0) {
			ImGui::End();
			return;
		}
		if (m_selectedGameObject->size() > 1) {

			if (ImGui::Button("Add Component")) {
				ImGui::OpenPopup("AddComponentModal");
			}
			AddComponentModal();

			ImGui::End();
			return;
		}

		GEngine::GameObject* gameObject = GEngine::GameObject::GetObjectFromHash(*m_selectedGameObject->begin());

		if (gameObject) {
			ImGui::Text("Tag: ");
			ImGui::SameLine();
			char buff[255] = { 0 };
			memcpy(buff, gameObject->GetTag().c_str(), gameObject->GetTag().size());
			if (ImGui::InputText("##tag: ", buff, 255, ImGuiInputTextFlags_EnterReturnsTrue)) {
				gameObject->SetTag(buff);
			}

			
			GEngine::Entity* entity = dynamic_cast<GEngine::Entity*>(gameObject);
			if (entity) {
				ImGui::Separator();
				ImVec2 p = ImGui::GetCursorScreenPos();
				bool b = ImGui::TreeNodeEx(("Transform Component : (" + entity->GetTransform()->GetHash().ToString() + ")").c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow);
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
					ComponentPayload payload = { entity->GetTransform() };
					ImGui::SetDragDropPayload(ComponentPayload::GetName(), (void*)&payload, sizeof(ComponentPayload));
					ImGui::SameLine();
					ImGui::Text(entity->GetTransform()->GetTag().c_str());
					ImGui::SameLine();
					ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
					ImGui::Text(("(" + entity->GetTransform()->GetHash().ToString() + ")").c_str());
					ImGui::PopStyleColor();
					ImGui::EndDragDropSource();
				}
				if (b) {
					s_ComponentMap["Transform Component"](entity->GetTransform());
					ImGui::TreePop();
				}
				//AcceptPayload(e, { pos.x,pos.y });
				

				for (const auto& p : entity->GetComponents()) {
					if (p.second->GetTag() == "Transform Component") continue;
					ImGui::Separator();
					std::string pName = (p.second->GetTag() + " : (" + p.second->GetHash().ToString() + ")");
					uint64_t flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow;
					if (!p.second->DoesSerialize()) {
						flags = ImGuiTreeNodeFlags_OpenOnArrow;
						ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
					}
					bool b = ImGui::TreeNodeEx(pName.c_str(), flags);
					if (!p.second->DoesSerialize()) {
						ImGui::PopStyleColor();
					}
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
						ComponentPayload payload = { p.second };
						ImGui::SetDragDropPayload(ComponentPayload::GetName(), (void*)&payload, sizeof(ComponentPayload));
						ImGui::SameLine();
						ImGui::Text(p.second->GetTag().c_str());
						ImGui::SameLine();
						ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
						ImGui::Text(("(" + p.second->GetHash().ToString() + ")").c_str());
						ImGui::PopStyleColor();
						ImGui::EndDragDropSource();
					}
					ImGui::SameLine();
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
					if (ImGui::ImageButton((ImTextureID)(uintptr_t)m_textures["remove"]->GetRendererID(), { ImGui::GetFontSize() / 2.f, ImGui::GetFontSize() / 2.f }, { 0,1 }, { 1,0 })) {
						GEngine::ThreadPool::AddMainThreadFunction([entity, p]() {
							entity->RemoveComponent(p.second);
							EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyEntity>(entity->GetHash(), GameObjectModifications::REMOVE_COMPONENT);
						});
						ImGui::PopStyleVar();
						ImGui::TreePop();
						break;
					}
					ImGui::PopStyleVar();
					if (b) {
						std::unordered_map < std::string, std::function<void(GEngine::Component*)> > ::iterator it = s_ComponentMap.find(p.second->GetTag());
						if (it != s_ComponentMap.end()) {
							it->second(p.second);
						}
						ImGui::TreePop();
					}
				}
			
			}

			ImGui::Separator();

			if (ImGui::Button("Add Component")) {
				ImGui::OpenPopup("AddComponentModal");
			}
			AddComponentModal();
		}

		if (ImGui::GetWindowDockNode())
			ImGui::GetWindowDockNode()->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;

		windowWidth = ImGui::GetWindowWidth();

		ImGui::End();
	}

	void InspectorModule::Inspect_TransformComponent(GEngine::Component* c)
	{
		GEngine::Transform* transform = dynamic_cast<GEngine::Transform*>(c);

		if (transform) {
			ImGui::BeginChild("TransformComponent", { ImGui::GetContentRegionAvailWidth(), 90.f }, false, ImGuiWindowFlags_HorizontalScrollbar);
			GEngine::Vector3f pos = transform->GetLocalPosition();
			GEngine::Vector3f rot = transform->GetLocalRotation();
			GEngine::Vector3f scale = transform->GetLocalScale();
			const std::unordered_map < std::string, GEngine::Vector3f*> transData = {
				{"Position", &pos},
				{"Rotation", &rot},
				{"Scale", &scale}
			};
			ImGui::Columns(2);
			//ImGui::PushFont(dynamic_cast<ImpulseEditor*>(GEngine::Application::GetApp())->mainFont);
			float fontSize = ImGui::GetFontSize();
			ImGui::BeginChild("TransformName", { 0.f,0.f }, false);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,0,0,0 });
			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
			ImGui::Button("Position");
			ImGui::Button("Rotation");
			ImGui::Button("Scale");
			ImGui::PopStyleColor(3);
			ImGui::PopStyleVar();
			//ImGui::PopFont();
			ImGui::EndChild();
			ImGui::NextColumn();
			ImGui::SetColumnOffset(-1, fontSize*6);
			ImGui::BeginChild("TransformInput", { 0.f,0.f }, false, ImGuiWindowFlags_HorizontalScrollbar);

			for (const auto& d : transData) {
				TransformWidget(d.first, d.second);
			}

			ImGui::EndChild();

			if (pos != transform->GetLocalPosition()) {
				transform->SetLocalPosition(pos);
				EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(transform->GetHash(), GameObjectModifications::EDIT_MEMBER);
			}
			if (scale != transform->GetLocalScale()) {
				transform->SetLocalScale(scale);
				EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(transform->GetHash(), GameObjectModifications::EDIT_MEMBER);
			}
			if (rot != transform->GetLocalRotation()) {
				transform->SetLocalRotation(rot);
				EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(transform->GetHash(), GameObjectModifications::EDIT_MEMBER);
			}


			ImGui::EndChild();
		}




	}


	void InspectorModule::Inspect_NativeComponent(GEngine::Component* c)
	{
		if (reloadModule->IsReloading()) return;
		
		GEngine::NativeScriptComponent* script = dynamic_cast<GEngine::NativeScriptComponent*>(c);

		if (script) {
			std::string clazz = script->GetClass();
			const auto& map = GEngine::ScriptApi::GetStorage_Native()->get_map();
			if (clazz.size() <= 0) {

				const std::string& str = nativeScriptComponentClass.size() <= 0 ? "None" : nativeScriptComponentClass;

				if (ImGui::BeginCombo("##scriptSelect", str.c_str())) {

					for (const auto& p : map) {
						if (std::find(p.second.parent_list.begin(), p.second.parent_list.end(), "Component") == p.second.parent_list.end()) continue;
						if (ImGui::Selectable(p.first.c_str())) {
							nativeScriptComponentClass = p.first;
						}
					}

					ImGui::EndCombo();
				}


				if (nativeScriptComponentClass != clazz) {
					if (ImGui::Button("Load Script")) {
						script->LoadClass(nativeScriptComponentClass);
						EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(script->GetHash(), GameObjectModifications::EDIT_MEMBER);
					}
				}
			}
			else {
				ImGui::Text(clazz.c_str());
				const auto& it = map.find(clazz);
				if (it != map.end()) {
					ImGui::Columns(2);
					const auto& params = it->second.property_map;
					// Left side
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
					for (const auto& p : params) {
						ImGui::Button(p.first.c_str());
					}
					ImGui::PopStyleColor(3);
					ImGui::PopStyleVar();
					ImGui::NextColumn();
					float sliderSpeed = .2f;
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
					// Right side
					for (const std::pair<std::string, refl::store::uproperty_struct>& p : params) {
						std::string idN = "##" + script->GetHash().ToString() + p.first;
						switch (p.second.type) {
						case refl::store::uproperty_type::_bool: {
							ImGui::Checkbox(idN.c_str(), &script->GetNativeObject()->GetMember<bool&>(p.first));
							break;
						}
						case refl::store::uproperty_type::_char:
						{
							int8_t* ptr = &script->GetNativeObject()->GetMember<int8_t&>(p.first);
							const int8_t min = -128;
							const int8_t max = 127;
							ImGui::DragScalar(idN.c_str(), ImGuiDataType_S8, ptr, sliderSpeed, &min, &max, "%d");
							break;
						}
						case refl::store::uproperty_type::_uchar: {
							uint8_t* ptr = &script->GetNativeObject()->GetMember<uint8_t&>(p.first);
							const uint8_t min = 0;
							const uint8_t max = 255;
							ImGui::DragScalar(idN.c_str(), ImGuiDataType_U8, ptr, sliderSpeed, &min, &max, "%u");
							break;
						}
						case refl::store::uproperty_type::_int: {
							int32_t* ptr = &script->GetNativeObject()->GetMember<int32_t&>(p.first);
							const int32_t min = -2147483648;
							const int32_t max = 2147483647;
							ImGui::DragScalar(idN.c_str(), ImGuiDataType_S32, ptr, sliderSpeed, &min, &max, "%d");
							break;
						}
						case refl::store::uproperty_type::_int8_t: {
							int8_t* ptr = &script->GetNativeObject()->GetMember<int8_t&>(p.first);
							const int8_t min = -128;
							const int8_t max = 127;
							ImGui::DragScalar(idN.c_str(), ImGuiDataType_S8, ptr, sliderSpeed, &min, &max, "%d");
							break;
						}
						case refl::store::uproperty_type::_int16_t: {
							int16_t* ptr = &script->GetNativeObject()->GetMember<int16_t&>(p.first);
							const int16_t min = -32768;
							const int16_t max = 32767;
							ImGui::DragScalar(idN.c_str(), ImGuiDataType_S16, ptr, sliderSpeed, &min, &max, "%d");
							break;
						}
						case refl::store::uproperty_type::_int32_t: {
							int32_t* ptr = &script->GetNativeObject()->GetMember<int32_t&>(p.first);
							const int32_t min = -2147483648;
							const int32_t max = 2147483647;
							ImGui::DragScalar(idN.c_str(), ImGuiDataType_S32, ptr, sliderSpeed, &min, &max, "%d");
							break;
						}
						case refl::store::uproperty_type::_int64_t: {
							int64_t* ptr = &script->GetNativeObject()->GetMember<int64_t&>(p.first);
							const int64_t min = -2147483648;
							const int64_t max = 2147483647;
							ImGui::DragScalar(idN.c_str(), ImGuiDataType_S64, ptr, sliderSpeed, &min, &max, "%lld");
							break;
						}
						case refl::store::uproperty_type::_uint: {
							uint32_t* ptr = &script->GetNativeObject()->GetMember<uint32_t&>(p.first);
							const uint32_t min = 0;
							const uint32_t max = 0xFFFFFFFF;
							ImGui::DragScalar(idN.c_str(), ImGuiDataType_U32, ptr, sliderSpeed, &min, &max, "%u");
							break;
						}
						case refl::store::uproperty_type::_uint8_t: {
							uint8_t* ptr = &script->GetNativeObject()->GetMember<uint8_t&>(p.first);
							const uint8_t min = 0;
							const uint8_t max = 255;
							ImGui::DragScalar(idN.c_str(), ImGuiDataType_U8, ptr, sliderSpeed, &min, &max, "%u");
							break;
						}
						case refl::store::uproperty_type::_uint16_t: {
							uint16_t* ptr = &script->GetNativeObject()->GetMember<uint16_t&>(p.first);
							const uint16_t min = 0;
							const uint16_t max = 65535;
							ImGui::DragScalar(idN.c_str(), ImGuiDataType_U16, ptr, sliderSpeed, &min, &max, "%u");
							break;
						}
						case refl::store::uproperty_type::_uint32_t: {
							uint32_t* ptr = &script->GetNativeObject()->GetMember<uint32_t&>(p.first);
							const uint32_t min = 0;
							const uint32_t max = 0xFFFFFFFF;
							ImGui::DragScalar(idN.c_str(), ImGuiDataType_U32, ptr, sliderSpeed, &min, &max, "%u");
							break;
						}
						case refl::store::uproperty_type::_uint64_t: {
							uint64_t* ptr = &script->GetNativeObject()->GetMember<uint64_t&>(p.first);
							const uint64_t min = 0;
							const uint64_t max = 0xFFFFFFFFFFFFFFFF;
							ImGui::DragScalar(idN.c_str(), ImGuiDataType_U64, ptr, sliderSpeed, &min, &max, "%llu");
							break;
						}
						case refl::store::uproperty_type::_float: {
							float* ptr = &script->GetNativeObject()->GetMember<float&>(p.first);
							const float min = 1.17549e-038;
							const float max = 3.40282e+038;
							ImGui::DragScalar(idN.c_str(), ImGuiDataType_Float, ptr, sliderSpeed, &min, &max, "%f");
							break;
						}
						case refl::store::uproperty_type::_double: 
						{
							double* ptr = &script->GetNativeObject()->GetMember<double&>(p.first);
							const double min = 2.22507e-308;
							const double max = 1.79769e+308;
							ImGui::DragScalar(idN.c_str(), ImGuiDataType_Double, ptr, sliderSpeed, &min, &max, "%lf");
							break;
						}
						case refl::store::uproperty_type::uclass_ref: {
							ImGui::Button((p.second.type_name + " uclass_ref") .c_str());
							break;
						}
						case refl::store::uproperty_type::uclass: {
							//ImGui::Button((p.second.type_name + " uclass").c_str());
							if (p.second.type_name == "std::string") {
								std::string* s = &script->GetNativeObject()->GetMember<std::string&>(p.first);
								memset(m_textBuffer, 0, sizeof(m_textBuffer));
								memcpy(m_textBuffer, s->data(), s->size() * sizeof(char));
								if (ImGui::InputText(idN.c_str(), m_textBuffer, sizeof(m_textBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
									*s = m_textBuffer;
								}
								
							}
							else if (p.second.type_name == "Vector3f") {
								GEngine::Vector3f* ptr = &script->GetNativeObject()->GetMember<GEngine::Vector3f&>(p.first);
								ImGui::DragFloat3(idN.c_str(), (float*)ptr->data(), 1.f);
							}
							else if (p.second.type_name == "Vector2f") {
								GEngine::Vector2f* ptr = &script->GetNativeObject()->GetMember<GEngine::Vector2f&>(p.first);
								ImGui::DragFloat2(idN.c_str(), (float*)ptr->data(), 1.f);
							}
							else {
								ImGui::Button((p.second.type_name + " uclass").c_str());
							}
							break;
						}
						case refl::store::uproperty_type::uclass_ptr: {
							std::string payloadType = "None";
							void* payloadData = nullptr;
							size_t payloadSize = 0;
							std::string s = p.second.type_name.substr(0, p.second.type_name.size() - 1);
							if (FindParent(s, "Component", map)) {
								GEngine::Component* ptr = dynamic_cast<GEngine::Component*>(script->GetNativeObject()->GetMember<GEngine::Component*>(p.first));
								std::string val = "nullptr";
								if (ptr) {
									val = ptr->GetHash().ToString();
								} 
								ImGui::Button(val.c_str());
								if (ImGui::BeginDragDropTarget()) {
									const ImGuiPayload* pay = ImGui::GetDragDropPayload();
									if (pay && ComponentPayload::GetName() == std::string(pay->DataType)) {
										const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(ComponentPayload::GetName());
										if (payload) {
											ComponentPayload* load = (ComponentPayload*)payload->Data;
											script->GetNativeObject()->SetMember<GEngine::Component*>(p.first, load->component);
											EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(script->GetHash(), GameObjectModifications::EDIT_MEMBER);
										}
									}
								}
							}
							else if (FindParent(s, "Entity", map)) {
								GEngine::Entity* ptr = dynamic_cast<GEngine::Entity*>(script->GetNativeObject()->GetMember<GEngine::Entity*>(p.first));
								std::string val = "nullptr";
								if (ptr) {
									val = ptr->GetHash().ToString();
								}
								ImGui::Button(val.c_str());
								if (ImGui::BeginDragDropTarget()) {
									const ImGuiPayload* pay = ImGui::GetDragDropPayload();
									if (pay && EntityPayload::GetName() == std::string(pay->DataType)) {
										const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(EntityPayload::GetName());
										if (payload) {
											EntityPayload* load = (EntityPayload*)payload->Data;
											script->GetNativeObject()->SetMember<GEngine::Entity*>(p.first, load->entity);
											EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(script->GetHash(), GameObjectModifications::EDIT_MEMBER);
										}
									}
								}
							}
							else if (FindParent(s, "GameObject", map)) {
								GEngine::GameObject* ptr = script->GetNativeObject()->GetMember<GEngine::GameObject*>(p.first);
								std::string val = "nullptr";
								if (ptr) {
									val = ptr->GetHash().ToString();
								}
								ImGui::Button(val.c_str());
								if (ImGui::BeginDragDropTarget()) {
									const ImGuiPayload* pay = ImGui::GetDragDropPayload();
									if (pay && GameObjectPayload::GetName() == std::string(pay->DataType)) {
										const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(GameObjectPayload::GetName());
										if (payload) {
											GameObjectPayload* load = (GameObjectPayload*)payload->Data;
											script->GetNativeObject()->SetMember<GEngine::GameObject*>(p.first, load->go);
											EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(script->GetHash(), GameObjectModifications::EDIT_MEMBER);
										}
									}

								}
							}
							else {
								ImGui::Button((s + " uclass_ptr").c_str());
							}
							
							
							break;
						}
						case refl::store::uproperty_type::_ptr: {
							char ch[32] = { 0 };
							memset(ch, 0, sizeof(ch));
							ch[0] = '0';
							ch[1] = 'x';
							void* ptr = script->GetNativeObject()->GetMember<void*>(p.first);
							sprintf(&ch[2], "%p", ptr);
							ImGui::InputText(idN.c_str(), ch, sizeof(char) * 16, ImGuiInputTextFlags_ReadOnly);
							break;
						}
						default:
							ImGui::Button(p.second.type_name.c_str());
						break;
						}
						
					}
					ImGui::PopStyleVar();
					const auto& parents = it->second.parent_list;
					ImGui::Text("Inheritance");
					for (const std::string& p : parents) {
						ImGui::Text(p.c_str());
					}
					ImGui::EndColumns();
				}
				
				
			}
		}

	}

	void InspectorModule::Inspect_SpriteComponent(GEngine::Component* c)
	{
		if (reloadModule->IsReloading()) return;
		GEngine::SpriteComponent* sprite = dynamic_cast<GEngine::SpriteComponent*>(c);
		if (sprite) {
			for (const auto& id : sprite->GetObjects()) {
				ImGui::Button(std::to_string(id.first).c_str());
			}
		}
	}

	void InspectorModule::Inspect_UIComponent(GEngine::Component* c)
	{
		GEngine::UIComponent* sprite = dynamic_cast<GEngine::UIComponent*>(c);
		if (sprite) {
			
			for (const auto& id : sprite->GetObjects()) {
				if (id.second.text) continue;
				if (ImGui::TreeNodeEx(("Quad (" + std::to_string(id.first) + ")").c_str(), ImGuiTreeNodeFlags_OpenOnArrow)) {
					ImGui::Columns(2);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
					ImGui::Button("Position");
					ImGui::Button("Rotation");
					ImGui::Button("Scale");
					ImGui::Button("Color");
					ImGui::Button("Texture");
					if (id.second.texture)
						ImGui::Button("Texture Scale");
					ImGui::Button("Aspect Ratio");
					ImGui::PopStyleColor(3);
					ImGui::PopStyleVar();
					ImGui::NextColumn();
					GEngine::Vector3f pos = id.second.pos;
					TransformWidget(std::string("##Position-" + std::to_string(id.first)), &pos);
					float rot = id.second.rot;
					ImGui::DragFloat(std::string("##Rot-" + std::to_string(id.first)).c_str(), &rot);
					GEngine::Vector3f scale = id.second.scale;
					TransformWidget(std::string("##Scale-" + std::to_string(id.first)), &scale);
					GEngine::Vector4f color = id.second.color;
					ImGui::ColorEdit4(std::string("##Color-" + std::to_string(id.first)).c_str(), (float*)color.data());
					std::string textureName = "None";
					if (id.second.texture) {
						textureName = id.second.texture->GetName();
					}
					else if (id.second.subtexture) {
						textureName = id.second.subtexture->GetTexture()->GetName();
					}
					ImGui::Button((char*)textureName.c_str());
					if (ImGui::BeginDragDropTarget()) {
						const ImGuiPayload* _p = ImGui::GetDragDropPayload();
						if (_p) {
							const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DirectoryPayload::GetName());
							if (payload) {
								GE_CORE_DEBUG("ACCEPTED PAYLOAD: {0}", payload->Data);
								DirectoryPayload payloadPath = *(DirectoryPayload*)payload->Data;
								textureName = payloadPath.path;
							}
						}
						ImGui::EndDragDropTarget();
					}

					GEngine::Vector2f textureScale = id.second.textureScale;
					if (textureName != "None") {
						
						ImGui::InputFloat2(("##textureAspect-" + std::to_string(id.first)).c_str(), (float*)textureScale.data());
					}

					bool scaled = id.second.aspectRatio;
					ImGui::Checkbox("##autoScale", &scaled);

					bool updated = false;
					if (pos.z != id.second.pos.z) {
						sprite->SetZOrder(id.first, pos.z);
						updated = true;
					}
					else if (pos != id.second.pos) {
						sprite->SetPosition(id.first, pos);
						updated = true;
					} else if (rot != id.second.rot) {
						sprite->SetRotation(id.first, rot);
						updated = true;
					} else
					if (scale != id.second.scale) {
						sprite->SetScale(id.first, scale);
						updated = true;
					}
					else if (color != id.second.color) {
						sprite->SetColor(id.first, color);
						updated = true;
					}
					else if (!id.second.texture && textureName != "None" || id.second.texture && textureName != id.second.texture->GetName()) {
						sprite->SetTexture(id.first, GEngine::Texture2D::Create(textureName));
						updated = true;
					}
					else if (id.second.aspectRatio != scaled) {
						sprite->SetAutoScale(id.first, scaled);
						updated = true;
					}
					else if (id.second.textureScale != textureScale)
					{
						sprite->SetTextureScale(id.first, textureScale);
						updated = true;
					}

					if (updated) {
						EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(sprite->GetHash(), GameObjectModifications::EDIT_MEMBER);
					}

					if (ImGui::Button("Remove"))
					{
						sprite->Remove(id.first);
						EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(sprite->GetHash(), GameObjectModifications::EDIT_MEMBER);
						ImGui::EndColumns();
						ImGui::TreePop();
						break;
					}

					ImGui::EndColumns();
					ImGui::TreePop();
				}
			}
			for (const auto& id : sprite->GetTexts()) {
				if (ImGui::TreeNodeEx(("Text ("+id.first+") (" + id.first + ")").c_str(), ImGuiTreeNodeFlags_OpenOnArrow)) {
					ImGui::Columns(2);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,0,0,0 });
					ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);
					ImGui::Button("Text");
					ImGui::Button("Position");
					ImGui::Button("Scale");
					ImGui::Button("Font");
					ImGui::PopStyleColor(3);
					ImGui::PopStyleVar();
					ImGui::NextColumn();
					char ch[2048];
					ch[2047] = 0;
					for (int i = 0; i < id.second.str.size(); i++) {
						char _c = (char)id.second.str[i];
						ch[i] = _c;
					}

					ch[id.second.str.size()] = 0;
					bool didChange = false;
					if (ImGui::InputText(("##inputText-" + id.first).c_str(), ch, sizeof(ch), ImGuiInputTextFlags_EnterReturnsTrue)) {
						sprite->SetText(id.first, ch, { 1.f,1.f,1.f,1.f });
						didChange = true;
					}
					GEngine::Vector3f pos = id.second.pos;
					TransformWidget(std::string("##Position-" + id.first), &pos);
					GEngine::Vector3f scale = id.second.scale;
					TransformWidget(std::string("##Scale-" + id.first), &scale);
					if (id.second.font)
						ImGui::Button(id.second.font->GetTexture()->GetName().c_str());
					else ImGui::Button("Null Texture");

					if (pos != id.second.pos) {
						sprite->SetPosition(id.first, pos);
					}

					if (pos.z != id.second.pos.z) {
						sprite->SetZOrder(id.first, pos.z);
					}
					if (scale != id.second.scale) {
						sprite->SetScale(id.first, scale);
					}
					if (!didChange) {
						for (int i = 0; i < id.second.shapes.size(); i++) {
							if (ImGui::TreeNodeEx(("Character (" + std::to_string((char)id.second.str[i]) + ") (" + std::to_string(id.second.shapes[i]) + ")").c_str(), ImGuiTreeNodeFlags_OpenOnArrow)) {
								GEngine::Vector4f col = sprite->GetObjects().at(id.second.shapes[i]).color;
								GEngine::Vector4f stCol = sprite->GetObjects().at(id.second.shapes[i]).color;
								ImGui::ColorEdit4(("##color" + std::to_string(id.second.shapes[i])).c_str(), (float*)col.data());
								if (col != stCol) {
									sprite->SetColor(id.second.shapes[i], col);
								}
								ImGui::TreePop();
							}
						}
					}

					if (ImGui::Button("Remove"))
					{
						sprite->RemoveText(id.first);
						EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(sprite->GetHash(), GameObjectModifications::EDIT_MEMBER);
						ImGui::EndColumns();
						ImGui::TreePop();
						break;
					}

					ImGui::EndColumns();
					ImGui::TreePop();
				}
			}
			if (ImGui::Button("Add Object")) {
				sprite->CreateQuad({ 0,0,0 }, 0.f, GEngine::Vector3f(1.f), GEngine::Vector4f(1.f), nullptr, true);
				EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(sprite->GetHash(), GameObjectModifications::EDIT_MEMBER);
			}

			if (ImGui::Button("Add Text")) {
				GEngine::Ref<GEngine::Font> font = GEngine::Font::Create("EngineContent/Fonts/roboto.ttf", 300.f);
				font->LoadCharactersEN();
				sprite->CreateText("Text",font, GEngine::Vector3f(0.f), GEngine::Vector3f(1.f), GEngine::Vector4f(1.f));
				EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(sprite->GetHash(), GameObjectModifications::EDIT_MEMBER);
			}
		}

	}

	void InspectorModule::ResizePanel(uint64_t id, bool& selfDrag, float& leftWidth, float leftWidthMax, float leftWidthMin) {
		ImVec2 pos = ImGui::GetCursorScreenPos();
		float height = ImGui::GetContentRegionAvail().y;
		ImGui::BeginChild((std::string("Resizeable")+std::to_string(id)).c_str(), { 2.5f, 0 }, 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::GetWindowDrawList()->AddRectFilled({ pos.x, pos.y }, { pos.x + 2.5f, pos.y + height }, IM_COL32(125, 125, 125, 125));
		if (ImGui::IsMouseHoveringRect({ pos.x, pos.y }, { pos.x + 5, pos.y + height })) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
			if (ImGui::IsMouseDown(0)) {
				selfDrag = true;
				isDragging = true;
				lastX = ImGui::GetMousePos().x;
			}
			else {
				if (selfDrag)
					isDragging = false;
				selfDrag = false;
			}
		}
		if (!ImGui::IsMouseDown(0)) {
			selfDrag = false;
		}
		else if (selfDrag && !isDragAndDrop) {
			float xPos = ImGui::GetMousePos().x;
			float xDist = xPos - lastX;
			if (xDist != 0) {

				xDist /= windowWidth;

				leftWidth += xDist;

				if (leftWidth > leftWidthMax) {
					leftWidth = leftWidthMax;
				}
				else if (leftWidth < leftWidthMin) {
					leftWidth = leftWidthMin;
				}

				lastX = xPos;
			}
		}
		ImGui::EndChild();
	}

	void InspectorModule::TransformWidget(const std::string& id, const GEngine::Vector3f* data)
	{
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1,0,0,1 });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 1,0,0,1 });
		ImGui::PushStyleColor(ImGuiCol_Button, { 1,0,0,1 });
		ImGui::Button("X");
		ImGui::PopStyleColor(3);
		ImGui::SameLine(0, 0);
		ImGui::SetNextItemWidth(65);
		ImGui::InputFloat(("##X" + id).c_str(), (float*)&(data->x), 0, 0, "%.2f");
		ImGui::SameLine(0, 0);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,1,0,1 });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,1,0,1 });
		ImGui::PushStyleColor(ImGuiCol_Button, { 0,1,0,1 });
		ImGui::Button("Y");
		ImGui::PopStyleColor(3);
		ImGui::SameLine(0, 0);
		ImGui::SetNextItemWidth(65);
		ImGui::InputFloat(("##Y" + id).c_str(), (float*)&(data->y), 0, 0, "%.2f");
		ImGui::SameLine(0, 0);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,0,1,1 });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,0,1,1 });
		ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,1,1 });
		ImGui::Button("Z");
		ImGui::PopStyleColor(3);
		ImGui::SameLine(0, 0);
		ImGui::SetNextItemWidth(65);
		ImGui::InputFloat(("##Z" + id).c_str(), (float*)&(data->z), 0, 0, "%.2f");

	}

	bool InspectorModule::FindParent(const std::string& clazz, const std::string& parent, const std::unordered_map<std::string, refl::store::uobject_struct>& map)
	{
		if (clazz == parent) return true;
		const auto& uobj = map.find(clazz);
		if (parent == "Component") {
			if (EngineComponents.find(clazz) != EngineComponents.end()) return true;
		}
		if (uobj == map.end()) return false;
		const std::vector<std::string> m = uobj->second.parent_list;
		std::queue<std::string> q;
		for (const auto& s : m) q.push(s);
		while (!q.empty()) {
			const std::string s = q.front();
			q.pop();
			if (s == parent) return true;
			const auto& it = map.find(s);
			if (it != map.end()) {
				const std::vector<std::string> _m = it->second.parent_list;
				for (const auto& s : _m) {
					q.push(s);
				}
			}

		}

		return false;
	}

	void InspectorModule::AddComponentModal()
	{
		if (ImGui::BeginPopup("AddComponentModal", ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoMove)) {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
			
			GEngine::Entity* entity = dynamic_cast<GEngine::Entity*>(GEngine::GameObject::GetObjectFromHash(*m_selectedGameObject->begin()));
			if (entity) {

				if (ImGui::Button("Sprite Component", { ImGui::GetContentRegionAvailWidth() ,0 })) {
					for (const auto& hash : *m_selectedGameObject) {
						GEngine::Entity* ent = dynamic_cast<GEngine::Entity*>(GEngine::GameObject::GetObjectFromHash(hash));
						if (ent) {
							GEngine::SpriteComponent* s = ent->AddComponent<GEngine::SpriteComponent>();
							s->SetSerialize(true);
						}
					}
					EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyEntity>(entity->GetHash(), GameObjectModifications::ADD_COMPONENT);
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("UI Component", { ImGui::GetContentRegionAvailWidth() ,0 })) {
					for (const auto& hash : *m_selectedGameObject) {
						GEngine::Entity* ent = dynamic_cast<GEngine::Entity*>(GEngine::GameObject::GetObjectFromHash(hash));
						if (ent) {
							GEngine::UIComponent* s = ent->AddComponent<GEngine::UIComponent>();
							s->SetSerialize(true);
						}
					}
					EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyEntity>(entity->GetHash(), GameObjectModifications::ADD_COMPONENT);
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("Native Script Component", { ImGui::GetContentRegionAvailWidth() ,0 })) {
					for (const auto& hash : *m_selectedGameObject) {
						GEngine::Entity* ent = dynamic_cast<GEngine::Entity*>(GEngine::GameObject::GetObjectFromHash(hash));
						if (ent) {
							GEngine::NativeScriptComponent* s = ent->AddComponent<GEngine::NativeScriptComponent>("");
							s->SetSerialize(true);

						}
					}
					EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyEntity>(entity->GetHash(), GameObjectModifications::ADD_COMPONENT);
					ImGui::CloseCurrentPopup();
				}

			}

			ImGui::PopStyleColor();

			ImGui::EndPopup();
		}

	}

}
#endif