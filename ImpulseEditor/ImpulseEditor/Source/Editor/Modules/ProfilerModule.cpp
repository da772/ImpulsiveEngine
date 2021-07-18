#include "ProfilerModule.h"
#include "imgui/imgui_internal.h"
#include "implot.h"
#include "implot_internal.h"


namespace Editor {

	ProfilerModule::ProfilerModule()
	{
		for (int i = 0; i < maxHistory; i++) {
			historyGraph.push_back(i+1);
		}

		
		
	}

	ProfilerModule::~ProfilerModule()
	{

	}

	static float d[3] = { 0 };

	static float fov = 30;

	void ProfilerModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{

		ImGui::Begin(name.c_str(), is_open, flags);
		ImGui::Text("FPS: %d", (int)GEngine::Application::GetApp()->profile["FPS"]);
		ImGui::Text("Run: %.3f MS", GEngine::Application::GetApp()->profile["Run"]);
		ImGui::Text("Draw: %.3f MS", GEngine::Application::GetApp()->profile["Draw"]);
		ImGui::Text("Render: %.3f MS", GEngine::Application::GetApp()->profile["Render"]);
		ImGui::Text("LayerDraw: %.3f MS", GEngine::Application::GetApp()->profile["LayerDraw"]);
		if (ImGui::GetWindowDockNode())
			ImGui::GetWindowDockNode()->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;

		uint64_t time = GEngine::Time::GetEpochTimeMS();
		if (time - lastUpdate > updateTime) {

			if (fpsHistory.size() >= maxHistory) {
				fpsHistory.erase(fpsHistory.begin());
			}
			int fps = (int)GEngine::Application::GetApp()->profile["FPS"];
			if (fps > fpsMax)
				fpsMax = fps;
			fpsHistory.push_back(fps);
			
			lastUpdate = time;
		}

		ImPlot::SetNextPlotLimitsX(1, maxHistory, ImGuiCond_Always);
		//ImPlot::SetNextPlotLimitsY(0, fpsMax+fpsMax*.35f, ImGuiCond_Always);
		if (ImPlot::BeginPlot("##Scrolling", NULL, NULL, ImVec2(250, 150), 0, 0, ImPlotAxisFlags_AutoFit)) {
			ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
			ImPlot::PlotShaded((std::to_string(fpsHistory[fpsHistory.size()-1])+" FPS").c_str(), &historyGraph.data()[0], &fpsHistory.data()[0], fpsHistory.size(), -INFINITY, 0, 1 * sizeof(uint64_t));
			ImPlot::SetNextLineStyle({ 0.f,255.f,0.f, 255.f }, 1.5f);
			ImPlot::PlotLine<uint64_t>((std::to_string(fpsHistory[fpsHistory.size() - 1]) + " FPS").c_str(), &historyGraph.data()[0], &fpsHistory.data()[0],fpsHistory.size(),0, sizeof(uint64_t));
			//ImPlot::PlotLine("Mouse Y", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), sdata2.Offset, 2 * sizeof(float));
			ImPlot::EndPlot();
		}

		ImGui::End();
	}

}