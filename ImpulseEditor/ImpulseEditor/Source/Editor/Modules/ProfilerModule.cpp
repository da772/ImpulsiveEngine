#include "ProfilerModule.h"
#include "imgui/imgui_internal.h"
#include "implot.h"
#include "implot_internal.h"


namespace Editor {

	ProfilerModule::ProfilerModule()
	{
		double d = 1.0;
		for (uint64_t i = 0; i < maxHistory; i++) {
			historyGraph.push_back(i+1);
			timingHistoryGraph.push_back(d);
			d += 1.0;
		}
		
		lastUpdate = GEngine::Time::GetEpochTimeMS();
		timingGraphs.push_back({ "Frame Time", "TotalFrame", {255.f, 0.f, 0.f, 255.f} });
		timingGraphs.push_back({ "Draw", "Draw", {255.f, 255.f, 0.f, 255.f} });
		timingGraphs.push_back({ "Update", "Run", {0.f, 255.f, 0.f, 255.f} });
		timingGraphs.push_back({ "View Render", "Render", {0.f, 0.f, 255.f, 255.f} });
		timingGraphs.push_back({ "ImGui Render", "LayerDraw", {255.f, 0.f, 255.f, 255.f} });
		
	}

	ProfilerModule::~ProfilerModule()
	{

	}

	static float d[3] = { 0 };

	static float fov = 30;

	void ProfilerModule::Create(const std::string& name, bool* is_open, uint32_t flags)
	{

		ImGui::Begin(name.c_str(), is_open, flags);
		/*
		ImGui::Text("FPS: %d", (int)GEngine::Application::GetApp()->profile["FPS"]);
		ImGui::Text("Run: %.3f MS", GEngine::Application::GetApp()->profile["Run"]);
		ImGui::Text("Draw: %.3f MS", GEngine::Application::GetApp()->profile["Draw"]);
		ImGui::Text("Render: %.3f MS", GEngine::Application::GetApp()->profile["Render"]);
		ImGui::Text("LayerDraw: %.3f MS", GEngine::Application::GetApp()->profile["LayerDraw"]);
		*/
		if (ImGui::GetWindowDockNode())
			ImGui::GetWindowDockNode()->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;

		uint64_t time = GEngine::Time::GetEpochTimeMS();
		if (time - lastUpdate > updateTime) {


			for (ProfilerTimingGraph& g : timingGraphs) {
				if (g.data.size() >= maxHistory) {
					g.data.erase(g.data.begin());
				}
				
				double data = (double)GEngine::Application::GetApp()->profile[g.dataName];

				g.data.push_back(data);
			}


			if (fpsHistory.size() >= maxHistory) {
				fpsHistory.erase(fpsHistory.begin());
			}
			int fps = (int)GEngine::Application::GetApp()->profile["FPS"];
			
			fpsHistory.push_back(fps);
			
			lastUpdate = time;
		}

		
		ImPlot::SetNextPlotLimitsX(1, maxHistory, ImGuiCond_Always);
		if (ImPlot::BeginPlot("Frame Timing", NULL, NULL, ImVec2(-1,250), 0, 0, ImPlotAxisFlags_AutoFit)) {
			ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
			for (const ProfilerTimingGraph& g : timingGraphs) {
				if (g.data.size() > 0) {
					std::string n = std::to_string(g.data[g.data.size() - 1]) + " " + g.displayName;
					//ImPlot::PlotShaded(n.c_str(), &timingHistoryGraph.data()[0], &g.data.data()[0], g.data.size(), -INFINITY, 0, 1 * sizeof(double));
					ImPlot::SetNextLineStyle({ g.color.r, g.color.g, g.color.b, g.color.a }, 1.5f);
					ImPlot::PlotLine<double>(n.c_str(), (double*)&timingHistoryGraph.data()[0], &g.data.data()[0], g.data.size(), 0, sizeof(double));
				}
			}
			//ImPlot::PlotLine("Mouse Y", &sdata2.Data[0].x, &sdata2.Data[0].y, sdata2.Data.size(), sdata2.Offset, 2 * sizeof(float));
			ImPlot::EndPlot();
		}
		ImPlot::SetNextPlotLimitsX(1, maxHistory, ImGuiCond_Always);
		if (ImPlot::BeginPlot("FPS", NULL, NULL, ImVec2(-1, 250), 0, 0, ImPlotAxisFlags_AutoFit)) {
			ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
			if (fpsHistory.size() > 0) {
				ImPlot::PlotShaded((std::to_string(fpsHistory[fpsHistory.size() - 1]) + " FPS").c_str(), &historyGraph.data()[0], &fpsHistory.data()[0], fpsHistory.size(), -INFINITY, 0, 1 * sizeof(uint64_t));
				ImPlot::SetNextLineStyle({ 0.f,255.f,0.f, 255.f }, 1.5f);
				ImPlot::PlotLine<uint64_t>((std::to_string(fpsHistory[fpsHistory.size() - 1]) + " FPS").c_str(), &historyGraph.data()[0], &fpsHistory.data()[0], fpsHistory.size(), 0, sizeof(uint64_t));
			}
			ImPlot::EndPlot();
		}

		ImGui::End();
	}

}