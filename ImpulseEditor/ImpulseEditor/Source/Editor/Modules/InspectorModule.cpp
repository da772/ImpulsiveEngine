#include "InspectorModule.h"
#include "imgui/imgui_internal.h"

namespace Editor {

	InspectorModule::InspectorModule()
	{

	}

	InspectorModule::~InspectorModule()
	{

	}

	void InspectorModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{
		ImGui::Begin(name.c_str(), is_open, flags);

		ImGui::Text("Hello World!");

		if (ImGui::GetWindowDockNode())
			ImGui::GetWindowDockNode()->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;
		ImGui::End();
	}

}