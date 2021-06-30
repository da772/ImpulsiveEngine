#include "ProfilerModule.h"
#include "imgui/imgui_internal.h"


namespace Editor {

	ProfilerModule::ProfilerModule()
	{

	}

	ProfilerModule::~ProfilerModule()
	{

	}

	void ProfilerModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{
		ImGui::Begin(name.c_str(), is_open, flags);
		ImGui::Text("FPS: %d", (int)GEngine::Application::GetApp()->profile["FPS"]);
		ImGui::Text("Run: %.3f MS", GEngine::Application::GetApp()->profile["Run"]);
		ImGui::Text("ImGui: %.3f MS", GEngine::Application::GetApp()->profile["ImGui"]);
		if (ImGui::GetWindowDockNode())
			ImGui::GetWindowDockNode()->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;
		ImGui::End();
	}

}