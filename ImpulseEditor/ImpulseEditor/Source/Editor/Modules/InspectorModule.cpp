#include "InspectorModule.h"
#include "Shared/ImpulseEditor.h"
#include "Editor/Modules/ReloadModule.h"
#include "imgui/imgui_internal.h"
#include "Editor/EditorLayer.h"
#include "Editor/Events/EditorSceneEvents.h"


namespace Editor {

	
	InspectorModule::InspectorModule(std::set<GEngine::ObjectHash>* selectedGameObject, ReloadModule* reloadModule) : m_selectedGameObject(selectedGameObject), reloadModule(reloadModule)
	{
		s_ComponentMap = 
		{
			{ "Transform Component", BIND_EVENT_FN(InspectorModule, Inspect_TransformComponent)},
			{"Native Script Component", BIND_EVENT_FN(InspectorModule, Inspect_NativeComponent)}
		};

	}

	InspectorModule::~InspectorModule()
	{

	}

	void InspectorModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{
		ImGui::Begin(name.c_str(), is_open, flags | ImGuiWindowFlags_HorizontalScrollbar);
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
				if (ImGui::TreeNodeEx(("Transform Component : ("+entity->GetTransform()->GetHash().ToString()+")").c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow)) {
					s_ComponentMap["Transform Component"](entity->GetTransform());
					ImGui::TreePop();
				}

				for (const auto& p : entity->GetComponents()) {
					if (p.second->GetTag() == "Transform Component") continue;
					ImGui::Separator();
					if (ImGui::TreeNodeEx((p.second->GetTag()+ " : (" + p.second->GetHash().ToString() +")").c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow)) {
						std::unordered_map < std::string, std::function<void(GEngine::Component*)> > ::iterator it = s_ComponentMap.find(p.second->GetTag());
						if (it != s_ComponentMap.end()) {
							it->second(p.second);
						}
						else {
							//ImGui::Text("Hello World!");
						}

						if (ImGui::Button("Remove Component")) {
							GEngine::ThreadPool::AddMainThreadFunction([entity, p]() {
								EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneDestroyComponent>(p.first);
								entity->RemoveComponent(p.second);
								EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyEntity>(entity->GetHash(), GameObjectModifications::REMOVE_COMPONENT);
								});
							
							ImGui::TreePop();
							break;
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
			ImGui::BeginChild("TransformComponent", { ImGui::GetContentRegionAvailWidth(), 85.f }, false, ImGuiWindowFlags_HorizontalScrollbar);
			GEngine::Vector3f pos = transform->GetLocalPosition();
			GEngine::Vector3f rot = transform->GetLocalRotation();
			GEngine::Vector3f scale = transform->GetLocalScale();
			const std::unordered_map < std::string, GEngine::Vector3f*> transData = {
				{"Position", &pos},
				{"Rotation", &rot},
				{"Scale", &scale}
			};

			//ImGui::PushFont(dynamic_cast<ImpulseEditor*>(GEngine::Application::GetApp())->mainFont);
			float fontSize = ImGui::GetFontSize();
			ImGui::BeginChild("TransformName", { ImGui::GetContentRegionAvailWidth() * (leftWidth_transform),0.f }, false);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + fontSize / 8.f);
			ImGui::Text("Position");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + fontSize / 8.f);
			ImGui::Text("Rotation");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + fontSize / 8.f);
			ImGui::Text("Scale");
			//ImGui::PopFont();
			ImGui::EndChild();

			ImGui::SameLine();
			ResizePanel(reinterpret_cast<uintptr_t>(c), isDragging_transform, leftWidth_transform, leftWidthMax_transform, leftWidthMin_transform);
			ImGui::SameLine();

			ImGui::BeginChild("TransformInput", { 0.f,0.f }, false, ImGuiWindowFlags_HorizontalScrollbar);

			for (const auto& d : transData) {
				
				//ImGui::Text((d.first + ":").c_str());
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1,0,0,1 });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 1,0,0,1 });
				ImGui::PushStyleColor(ImGuiCol_Button, { 1,0,0,1 });
				ImGui::Button("X");
				ImGui::PopStyleColor(3);
				ImGui::SameLine(0,0);
				ImGui::SetNextItemWidth(65);
				ImGui::InputFloat(("##X"+d.first).c_str(), &(d.second->x), 0, 0, "%.2f");
				ImGui::SameLine(0,0);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,1,0,1 });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,1,0,1 });
				ImGui::PushStyleColor(ImGuiCol_Button, { 0,1,0,1 });
				ImGui::Button("Y");
				ImGui::PopStyleColor(3);
				ImGui::SameLine(0,0);
				ImGui::SetNextItemWidth(65);
				ImGui::InputFloat(("##Y" + d.first).c_str(), &(d.second->y), 0, 0, "%.2f");
				ImGui::SameLine(0,0);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,0,1,1 });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,0,1,1 });
				ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,1,1 });
				ImGui::Button("Z");
				ImGui::PopStyleColor(3);
				ImGui::SameLine(0,0);
				ImGui::SetNextItemWidth(65);
				ImGui::InputFloat(("##Z" + d.first).c_str(), &(d.second->z), 0, 0, "%.2f");
				
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
			if (clazz.size() <= 0) clazz = "EMPTY";

			ImGui::Text(clazz.c_str());

			const auto& map = GEngine::ScriptApi::GetStorage_Native()->get_map();

			const std::string& str = nativeScriptComponentClass.size() <= 0 ? "None" : nativeScriptComponentClass;

			if (ImGui::BeginCombo("##scriptSelect", str.c_str())) {

				for (const auto& p : map) {
					if (std::find(p.second.parent_list.begin(), p.second.parent_list.end(), "NativeScript") == p.second.parent_list.end()) continue;
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
			else {
				const auto& it = map.find(clazz);
				if (it != map.end()) {
					const auto& params = it->second.property_map;
					for (const auto& p : params) {

						if ((uint32_t)p.second.type >= (uint32_t)refl::store::uproperty_type::_int && (uint32_t)p.second.type <= (uint32_t)refl::store::uproperty_type::_uint64_t) {
							if (ImGui::InputInt(p.first.c_str(), &script->GetNativeObject()->GetMember<int>(p.second.name), 1, 100)) {
								EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(script->GetHash(), GameObjectModifications::EDIT_MEMBER);
							}
						}
						else if ((uint32_t)p.second.type == (uint32_t)refl::store::uproperty_type::_float) {
							if (ImGui::InputFloat(p.first.c_str(), &script->GetNativeObject()->GetMember<float>(p.second.name))) {
								EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(script->GetHash(), GameObjectModifications::EDIT_MEMBER);
							}
						}
						else if (p.second.type_name == "GEngine::Entity*") {
							char buff[255] = { 0 };
							GEngine::GameObject* ptr = script->GetNativeObject()->GetMember<GEngine::GameObject*>(p.second.name);

							if (ptr) {
								memcpy(buff, (ptr->GetTag() + " (" + ptr->GetHash().ToString() + ")").c_str(), ptr->GetTag().size() + 3 + ptr->GetHash().ToString().size());
								ImGui::InputText(p.first.c_str(), buff, ImGuiInputTextFlags_ReadOnly);
							}
							else {
								memcpy(buff, "nullptr", sizeof("nullptr"));
								ImGui::InputText(p.first.c_str(), buff, ImGuiInputTextFlags_ReadOnly);
							}

							if (ImGui::BeginDragDropTarget()) {
								const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("EntityGameObjectID");
								if (payload) {
									EntityPayload payloadObj = *(EntityPayload*)payload->Data;
									GEngine::Entity* child = payloadObj.entity;

									script->GetNativeObject()->SetMember<GEngine::Entity*>(p.second.name, child);
									EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(script->GetHash(), GameObjectModifications::EDIT_MEMBER);
								}
								ImGui::EndDragDropTarget();
							}
						}
						else if (p.second.type_name == "std::string") {

							char buff[2048] = { 0 };
							std::string s = script->GetNativeObject()->GetMember<std::string&>(p.second.name);
							memcpy(buff, s.c_str(), s.size());
							ImGui::InputText(p.first.c_str(), buff, ImGuiInputTextFlags_ReadOnly);

							if (ImGui::BeginDragDropTarget()) {
								const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DirectoryPath");
								if (payload) {
									DirectoryPayload payloadObj = *(DirectoryPayload*)payload->Data;
									GE_CORE_DEBUG("IS DIR {0} EXT: {1}", payloadObj.is_directory, payloadObj.ext);
									if (!payloadObj.is_directory && strcmp(payloadObj.ext, ".png") == 0) {
										std::string str = payloadObj.path;
										script->GetNativeObject()->SetMember<std::string>(p.second.name, str);
										EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(script->GetHash(), GameObjectModifications::EDIT_MEMBER);
									}

								}
								ImGui::EndDragDropTarget();
							}
						}
						else if (p.second.type == refl::store::uproperty_type::uclass_ptr) {
							char buff[255] = { 0 };
							GEngine::GameObject* ptr = dynamic_cast<GEngine::Component*>(script->GetNativeObject()->GetMember<GEngine::GameObject*>(p.second.name));

							if (ptr) {
								memcpy(buff, (ptr->GetTag() + " (" + ptr->GetHash().ToString() + ")").c_str(), ptr->GetTag().size() + 3 + ptr->GetHash().ToString().size());
								ImGui::InputText(p.first.c_str(), buff, ImGuiInputTextFlags_ReadOnly);
							}
							else {
								memcpy(buff, "nullptr", sizeof("nullptr"));
								ImGui::InputText(p.first.c_str(), buff, ImGuiInputTextFlags_ReadOnly);
							}

							if (ImGui::BeginDragDropTarget()) {
								const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ComponentGameObjectID");
								if (payload) {
									ComponentPayload payloadObj = *(ComponentPayload*)payload->Data;
									GEngine::Component* child = payloadObj.component;

									script->GetNativeObject()->SetMember<GEngine::Component*>(p.second.name, child);
									GEngine::ScriptApi::SetNativeScriptPtrs(script->GetComponent(), script->GetNativeObject());
									EditorLayer::GetDispatcher()->BroadcastEvent<EditorSceneModifyComponent>(script->GetHash(), GameObjectModifications::EDIT_MEMBER);

								}
								ImGui::EndDragDropTarget();
							}
						}
						else {
							ImGui::Text((p.first + " : " + p.second.type_name).c_str());
						}

					}

					const auto& parents = it->second.parent_list;
					ImGui::Text("Inheritance");
					for (const std::string& p : parents) {
						ImGui::Text(p.c_str());
					}
				}
				
				
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

	void InspectorModule::AddComponentModal()
	{
		if (ImGui::BeginPopup("AddComponentModal", ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoMove)) {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
			
			GEngine::Entity* entity = dynamic_cast<GEngine::Entity*>(GEngine::GameObject::GetObjectFromHash(*m_selectedGameObject->begin()));
			if (entity) {

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