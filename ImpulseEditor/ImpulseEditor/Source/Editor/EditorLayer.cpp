#include "EditorLayer.h"

namespace Editor {

	EditorLayer* EditorLayer::s_singleton = nullptr;

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
		
		directoryModule = new DirectoryModule(m_projectData.path + "/" + m_projectData.name + "/"+m_projectData.name+"/"+m_projectData.name+"/Content/");

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
		directoryModule->Create( nullptr, 0);
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
