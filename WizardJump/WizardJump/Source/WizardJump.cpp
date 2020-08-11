#include "WizardJump.h"
#include <glm/gtc/matrix_transform.hpp>
#include "SplashScreenScene.h"
#include "MainGameScene.h"

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
	GEngine::SceneManager::AddScene("splashScreen",s);
	GEngine::Ref<MainGameScene> s2 = GEngine::CreateGameObject<MainGameScene>("mainGame", nullptr);
	GEngine::SceneManager::AddScene("mainGame", s2);
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

WizardJump::WizardJump()
{
	this->m_width = 1280;
	this->m_height = 720;
	this->title = "WizardJump";

	GEngine::FileSystem::PakDirectory(GEngine::FileSystem::GetParentExecuteableDir(3)+"WizardJump/Content",
		GEngine::FileSystem::FilePath("Data/Content.pak"), false);
	GEngine::FileSystem::LoadPak("Data/Content.pak");
	GEngine::FileSystem::Copy(GEngine::FileSystem::FilePath("Data/Content.pak"), 
		GEngine::FileSystem::GetParentExecuteableDir(3) + "WizardJump/Data/Content.pak", false);

	SetGraphicsApi(GetDefaultGraphicsApi());
	SetWindowApi(GetDefaultWindowApi());

#if defined(GE_CONSOLE_APP) && !defined(GE_Dist)
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

void WizardJump::OnCleanDirtyApi()
{
	m_ExampleLayer->OnDetach();
	m_DebugLayer->OnDetach();
	m_ExampleLayer->OnAttach();
	m_DebugLayer->OnAttach();
}

void WizardJump::OnUpdate(GEngine::Timestep timeStep)
{

}
