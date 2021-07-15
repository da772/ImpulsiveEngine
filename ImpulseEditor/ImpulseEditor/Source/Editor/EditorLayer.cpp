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
#include "Modules/ReloadModule.h"
#include "Modules/ToolbarModule.h"
#include "Modules/InfoPanelModule.h"


#include "Shared/ImpulseEditor.h"
#include "Shared/Scene/EditorScene.h"

namespace Editor {

	EditorLayer* EditorLayer::s_singleton = nullptr;
	EditorDispatcher EditorLayer::s_dispatcher = EditorDispatcher();

	EditorLayer* EditorLayer::Create(Project::LocalProject* data)
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

	EditorLayer::EditorLayer(const std::string& name, Project::LocalProject* data) : Layer(name), m_projectData(*data)
	{
		if (!s_singleton)
			s_singleton = this;



		GE_CORE_INFO("PROJECT DATA: {0}, {1}", m_projectData.data.name, m_projectData.data.path);
		reloadModule = new ReloadModule(&m_projectData);
		reloadModule->Reload();
		AddModule<MainMenuModule>("MainMenu", true, ImGuiWindowFlags_AlwaysAutoResize, false, &modules, &m_projectData, reloadModule);
		AddModule<ToolbarModule>("Toolbar", true, ImGuiWindowFlags_AlwaysAutoResize, false, reloadModule);
		AddModule<DockModule>("Dock", true, ImGuiWindowFlags_AlwaysAutoResize, false, std::vector < std::pair < std::string, std::string>>());
		AddModule<DirectoryModule>("Content Browser", true, ImGuiWindowFlags_AlwaysAutoResize, true, m_projectData.data.path + "/" + m_projectData.data.name + "/"+m_projectData.data.name+"/"+m_projectData.data.name, &m_projectData.data);
		AddModule<ConsoleModule>("Console Log", true, ImGuiWindowFlags_AlwaysAutoResize, true);
		AddModule<ProfilerModule>("Profiler", true, ImGuiWindowFlags_AlwaysAutoResize, true);
		AddModule<InspectorModule>("Inspector", true, ImGuiWindowFlags_AlwaysAutoResize, true, &selectedGameObject, reloadModule);
        AddModule<ViewportModule>("Viewport", true, ImGuiWindowFlags_AlwaysAutoResize, false, "viewport", reloadModule);
        AddModule<HierarchyModule>("Hierarchy", true, ImGuiWindowFlags_AlwaysAutoResize, true, &selectedGameObject);
		AddModule<InfoPanelModule>("InfoPanel", true, ImGuiWindowFlags_AlwaysAutoResize, false);


		GE_CORE_DEBUG("EDITOR LAYER CREATED");
	}

	EditorLayer::~EditorLayer()
	{
		if (s_singleton == this)
			s_singleton = nullptr;
		delete reloadModule;
		GE_CORE_DEBUG("EDITOR LAYER DESTROYED");
	}

	void EditorLayer::Begin()
	{


	}


	void EditorLayer::OnAttach()
	{
		GEngine::Application::GetWindow()->MaximizeWindow();
		GEngine::Application::GetWindow()->SetTitle(m_projectData.data.name + " - Impulse Editor [EXPERIMENTAL VERSION]");
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
		
		ImGui::PushFont(((ImpulseEditor*)GEngine::Application::GetApp() )->smallFont);
		ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 5.f);
		for (const auto & id : modules_order) {
			std::pair<std::string, EditorModuleData&> d = { id, modules[id] };
			if (!d.second.isOpen) continue;
			d.second.data->Create(d.first, &d.second.isOpen, d.second.flags);
		}

		ImGui::PopStyleVar();
		ImGui::PopFont();

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
