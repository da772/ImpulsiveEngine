#include "HierarchyModule.h"

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

				if (ImGui::TreeNodeEx(std::to_string(e.first).c_str(), 0, "%lu", e.first)) {
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}

		ImGui::End();
	}

}