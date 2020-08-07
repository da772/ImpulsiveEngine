#include "Sandbox.h"
#include <glm/gtc/matrix_transform.hpp>
#include "SplashScreenScene.h"
#include "MainGameScene.h"
#include "EmptyScene.h"

ExampleLayer::ExampleLayer()

: Layer("ExampleLayer"), app(GEngine::Application::GetApp())
{



}

ExampleLayer::~ExampleLayer()
{

}

void ExampleLayer::OnImGuiRender()
{

}

void ExampleLayer::OnAttach()
{
	GEngine::Ref<SplashScreenScene> s = GEngine::CreateGameObject<SplashScreenScene>("splashScreen", nullptr);
	GEngine::SceneManager::AddScene("splashScreen", s);
	GEngine::Ref<MainGameScene> s2 = GEngine::CreateGameObject<MainGameScene>("mainGame", nullptr);
	GEngine::SceneManager::AddScene("mainGame", s2);
	GEngine::Ref<EmptyScene> s3 = GEngine::CreateGameObject<EmptyScene>("emptyScene", nullptr);
	GEngine::SceneManager::AddScene("emptyScene", s3);
	GEngine::SceneManager::SetCurrentScene("splashScreen");
}

void ExampleLayer::OnDetach()
{
	
}

void ExampleLayer::OnUpdate(GEngine::Timestep timeStep)
{

}

void ExampleLayer::OnDraw()
{
	
}

void ExampleLayer::OnEvent(GEngine::Event& event)
{

}

Sandbox::Sandbox()
{
	this->m_width = 540;
	this->m_height = 960;
	this->title = "Hello World";

#ifdef GE_CONSOLE_APP
	GEngine::FileSystem::PakDirectory(GEngine::FileSystem::GetParentExecuteableDir(3)+"Prototype/Content",
		GEngine::FileSystem::FilePath("Data/Content.pak"), false);
    GEngine::FileSystem::Copy(GEngine::FileSystem::FilePath("Data/Content.pak"),
        GEngine::FileSystem::GetParentExecuteableDir(3) + "Prototype/Data/Content.pak", false);
#endif
	GEngine::FileSystem::LoadPak("Data/Content.pak");


	//GEngine::FileSystem::UnloadPak();

	SetGraphicsApi(GetDefaultGraphicsApi());
	SetWindowApi(GetDefaultWindowApi());

#ifdef GE_CONSOLE_APP
	EnableImGui(true);
#else
	EnableImGui(false);
#endif
	
	GetWindow()->SetVSync(true);

	m_ExampleLayer = new ExampleLayer();
	m_DebugLayer = new DebugLayer();
	PushLayer(m_DebugLayer);
	PushLayer(m_ExampleLayer);
}

void Sandbox::OnCleanDirtyApi()
{
	m_ExampleLayer->OnDetach();
	m_DebugLayer->OnDetach();
	m_ExampleLayer->OnAttach();
	m_DebugLayer->OnAttach();
}

void Sandbox::OnUpdate(GEngine::Timestep timeStep)
{

}
