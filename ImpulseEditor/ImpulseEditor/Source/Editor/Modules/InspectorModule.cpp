#include "InspectorModule.h"
#include "imgui/imgui_internal.h"

namespace Editor {

	static std::unordered_map<std::string, void(*)(GEngine::Component*)> s_ComponentMap = {

		{"Transform Component", InspectorModule::Inspect_TransformComponent}

	};

	InspectorModule::InspectorModule(uint64_t* selectedGameObject) : m_selectedGameObject(selectedGameObject)

	{

	}

	InspectorModule::~InspectorModule()
	{

	}

	void InspectorModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{
		ImGui::Begin(name.c_str(), is_open, flags);
		GEngine::GameObject* gameObject = GEngine::GameObject::GetObjectFromHash(*m_selectedGameObject);



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
				if (ImGui::TreeNodeEx("Transform Component", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow)) {
					s_ComponentMap["Transform Component"](entity->GetTransform());
					ImGui::TreePop();
				}
				for (const auto& p : entity->GetComponents()) {
					if (p.second->GetTag() == "Transform Component") continue;
					ImGui::Separator();
					if (ImGui::TreeNodeEx(p.second->GetTag().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow)) {
						std::unordered_map<std::string, void(*)(GEngine::Component*)>::iterator it = s_ComponentMap.find(p.second->GetTag());
						if (it != s_ComponentMap.end()) {
							it->second(p.second);
						}
						else {
							ImGui::Text("Hello World!");
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
		ImGui::End();
	}

	void InspectorModule::Inspect_TransformComponent(GEngine::Component* c)
	{
		GEngine::Transform* transform = dynamic_cast<GEngine::Transform*>(c);

		if (transform) {
			GEngine::Vector3f pos = transform->GetLocalPosition();
			GEngine::Vector3f rot = transform->GetLocalRotation();
			GEngine::Vector3f scale = transform->GetLocalScale();
			const std::unordered_map < std::string, GEngine::Vector3f*> transData = {
				{"Position", &pos},
				{"Rotation", &rot},
				{"Scale", &scale}
			};

			for (const auto& d : transData) {
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2);
				ImGui::Text((d.first + ":").c_str());
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 1,0,0,1 });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 1,0,0,1 });
				ImGui::PushStyleColor(ImGuiCol_Button, { 1,0,0,1 });
				ImGui::Button("X");
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::SetNextItemWidth(65);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 7);
				ImGui::InputFloat(("##X"+d.first).c_str(), &(d.second->x), 0, 0, "%.3f");
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,1,0,1 });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,1,0,1 });
				ImGui::PushStyleColor(ImGuiCol_Button, { 0,1,0,1 });
				ImGui::Button("Y");
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::SetNextItemWidth(65);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 7);
				ImGui::InputFloat(("##Y" + d.first).c_str(), &(d.second->y));
				ImGui::SameLine();
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0,0,1,1 });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0,0,1,1 });
				ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,1,1 });
				ImGui::Button("Z");
				ImGui::PopStyleColor(3);
				ImGui::SameLine();
				ImGui::SetNextItemWidth(65);
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 7);
				ImGui::InputFloat(("##Z" + d.first).c_str(), &(d.second->z));
				ImGui::PopStyleVar();
			}


			if (pos != transform->GetLocalPosition()) {
				transform->SetLocalPosition(pos);
			}
			if (scale != transform->GetLocalScale()) {
				transform->SetLocalScale(scale);
			}
			if (rot != transform->GetLocalRotation()) {
				transform->SetLocalRotation(rot);
			}
		}
	}

	void InspectorModule::AddComponentModal()
	{
		if (ImGui::BeginPopup("AddComponentModal", ImGuiWindowFlags_AlwaysUseWindowPadding)) {
			ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
			if (ImGui::Button("Hello World", { ImGui::GetContentRegionAvailWidth() ,0 })) {

			}


			if (ImGui::Button("Hello", { ImGui::GetContentRegionAvailWidth() ,0 })) {

			}

			if (ImGui::Button("Swag", { ImGui::GetContentRegionAvailWidth() ,0 })) {

			}


			ImGui::PopStyleColor();

			ImGui::EndPopup();
		}

	}

}