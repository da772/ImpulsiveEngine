#include "ProfilerModule.h"
#include "imgui/imgui_internal.h"


namespace Editor {

	ProfilerModule::ProfilerModule()
	{

	}

	ProfilerModule::~ProfilerModule()
	{

	}

	static float d[3] = { 0 };

	static float fov = 30;

	void ProfilerModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{

		ImGui::Begin("CAMERA CONTROLS");

		GEngine::CameraController* cl = GEngine::Application::GetApp()->GetTargetCameraController();

		ImGui::InputFloat3("pos", d);

		cl->SetPosition(GEngine::Vector3f(d[0], d[1], d[2]));

		float f = 0;

		ImGui::InputFloat("ZOOM", &fov);

		cl->SetCameraZoom(fov);

		
		ImGui::End();

		ImGui::Begin(name.c_str(), is_open, flags);
		ImGui::Text("FPS: %d", (int)GEngine::Application::GetApp()->profile["FPS"]);
		ImGui::Text("Run: %.3f MS", GEngine::Application::GetApp()->profile["Run"]);
		ImGui::Text("Draw: %.3f MS", GEngine::Application::GetApp()->profile["Draw"]);
		ImGui::Text("Render: %.3f MS", GEngine::Application::GetApp()->profile["Render"]);
		ImGui::Text("LayerDraw: %.3f MS", GEngine::Application::GetApp()->profile["LayerDraw"]);
		if (ImGui::GetWindowDockNode())
			ImGui::GetWindowDockNode()->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;
		ImGui::End();
	}

}