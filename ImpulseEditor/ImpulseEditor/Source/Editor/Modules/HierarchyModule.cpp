#include "HierarchyModule.h"
#include "imgui/imgui_internal.h"

namespace Editor {


	HierarchyModule::HierarchyModule()
	{

	}

	HierarchyModule::~HierarchyModule()
	{

	}

	void HierarchyModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{
		ImGui::Begin(name.c_str(), is_open, flags);

		GEngine::Scene* scene = GEngine::SceneManager::GetCurrentScene();
		
		if (ImGui::TreeNode("Scene")) {

			for (const auto& e : scene->GetEntities()) {
				char ch[9] = { 0 };
				memcpy(ch, &e.first, sizeof(e.first));
				GEngine::GameObject* obj = GEngine::GameObject::GetObjectFromHash(e.first);
				
				if (ImGui::TreeNodeEx(obj->GetTag().c_str(), 0, "%s", obj->GetTag().c_str())) {
					for (GEngine::Component* c : e.second->GetComponents()) {
						if (ImGui::TreeNodeEx((void*)c->GetHash(), 0, "%s", c->GetTag().c_str())) {
							ImGui::TreePop();
						}
					}
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}

		if (ImGui::GetWindowDockNode())
			ImGui::GetWindowDockNode()->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;
		ImGui::End();
	}

}