#include "EditorLayer.h"
#include "Modules/DockModule.h"
#include "Modules/DirectoryModule.h"
#include "Modules/ProfilerModule.h"
#include "Modules/DockModule.h"
#include "Modules/ViewportModule.h"
#include "Modules/ConsoleModule.h"
#include "Modules/MainMenuModule.h"
#include "Modules/HierarchyModule.h"
#include "Modules/InspectorModule.h"

#include "Scene/EditorScene.h"

namespace Editor {

	EditorLayer* EditorLayer::s_singleton = nullptr;
	EditorDispatcher EditorLayer::s_dispatcher = EditorDispatcher();

	EditorLayer* EditorLayer::Create(Project::ProjectData* data)
	{
		if (s_singleton) return nullptr;

		return new EditorLayer("EditorLayer", data);
	}

	void EditorLayer::Destroy()
	{
		if (s_singleton) {
			delete s_singleton;
			s_singleton = nullptr;
		}
	}

	EditorLayer::EditorLayer(const std::string& name, Project::ProjectData* data) : Layer(name), m_projectData(data)
	{
		if (!s_singleton)
			s_singleton = this;

		GE_CORE_INFO("PROJECT DATA: {0}, {1}", m_projectData.name, m_projectData.path);

		AddModule<DirectoryModule>("Content Browser", true, 0, true, m_projectData.path + "/" + m_projectData.name + "/"+m_projectData.name+"/"+m_projectData.name);
		AddModule<ConsoleModule>("Console Log", true, 0, true);
		AddModule<ProfilerModule>("Profiler", true, 0, true);
		AddModule<InspectorModule>("Inspector", true, 0, true);
        AddModule<ViewportModule>("Viewport", true, 0, false, "viewport");
        AddModule<HierarchyModule>("Hierarchy", true, 0, true);
		AddModule<DockModule>("Dock", true, 0, false, std::vector < std::pair < std::string, std::string>>());
		AddModule<MainMenuModule>("MainMenu", true, 0, false, &modules);
		
		GE_CORE_DEBUG("EDITOR LAYER CREATED");
	}

	EditorLayer::~EditorLayer()
	{
		if (s_singleton == this)
			s_singleton = nullptr;

		GE_CORE_DEBUG("EDITOR LAYER DESTROYED");
	}

	void EditorLayer::Begin()
	{
		
	


	}

	void EditorLayer::OnAttach()
	{
		GEngine::Application::GetWindow()->MaximizeWindow();
		GEngine::Application::GetWindow()->SetTitle(m_projectData.name + " | Impulse Editor");
		GEngine::SceneManager::AddScene<EditorScene>("EditorScene");
		GEngine::SceneManager::SetCurrentScene("EditorScene", false);
		
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(GEngine::Timestep timeStep)
	{

	}

	void EditorLayer::OnEvent(GEngine::Event& event)
	{

	}

	void EditorLayer::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 5.f);
		for (auto & d : modules) {
			if (!d.second.isOpen) continue;
			d.second.data->Create(d.first, &d.second.isOpen, d.second.flags);
		}
		ImGui::PopStyleVar();

		//GEngine::Log::GetImGuiLog()->Draw("Console Log", 0);
	}

	void EditorLayer::OnDraw()
	{

	}

	void EditorLayer::End()
	{

	}

	EditorLayer* EditorLayer::GetEditor()
	{
		return s_singleton;
	}


}
