#include "EditorLayer.h"


EditorLayer* EditorLayer::s_singleton = nullptr;

EditorLayer* EditorLayer::Create()
{
	if (s_singleton) return nullptr;

	return new EditorLayer("EditorLayer");
}

void EditorLayer::Destroy()
{
	if (s_singleton) {
		delete s_singleton;
		s_singleton = nullptr;
	}
}

EditorLayer::EditorLayer(const std::string& name) : Layer(name)
{
	if (!s_singleton)
		s_singleton = this;
	else 
		delete this; // ??

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
