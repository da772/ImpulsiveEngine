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
#ifdef GE_CONSOLE_APP
	this->m_width = 540;
	this->m_height = 960;
	this->title = "WizardJump";
#if !defined GE_DIST
	GEngine::FileSystem::PakDirectory(GEngine::FileSystem::GetParentExecuteableDir(3)+"WizardJump/Content",
		GEngine::FileSystem::FilePath("Data/Content.pak"), false);
#endif
#endif
	GEngine::FileSystem::LoadPak("Data/Content.pak");
	GEngine::FileSystem::Copy(GEngine::FileSystem::FilePath("Data/Content.pak"), 
		GEngine::FileSystem::GetParentExecuteableDir(3) + "WizardJump/Data/Content.pak", false);

	s_debugTools = true;
	if (s_debugTools) {
		this->m_width = 1280;
		this->m_height = 720;
	}
	SetGraphicsApi(GetDefaultGraphicsApi());
	SetWindowApi(GetDefaultWindowApi());

	if (s_debugTools) {
		this->m_viewPortWidth = 1080;
		this->m_viewPortHeight = 1920;
		GEngine::RenderPipeline::GetFrameBuffer()->UpdateSize(m_viewPortWidth, m_viewPortHeight);
	}


#if defined(GE_CONSOLE_APP) && !defined(GE_DIST)
	EnableImGui(true);
#else
	EnableImGui(false);
#endif
	
	GetWindow()->SetVSync(true);

#ifdef GE_MOBILE_APP
	GEngine::AdManager::Initialize();
	GEngine::AdManager::SetUserId("This Is My User ID!");
#ifdef GE_PLATFORM_ANDROID

	GEngine::AdManager::SetAdId("ca-app-pub-0400118858384122~7825957542");

	// Google Test Id
	GEngine::AdManager::SetRewardAdId("ca-app-pub-3940256099942544/5224354917");


	// Prototype Ad
	//GEngine::AdManager::SetRewardAdId("ca-app-pub-4619437690188394/1929986237");
#endif
#ifdef GE_PLATFORM_IOS
	GEngine::AdManager::SetAdId("ca-app-pub-7573801306023183~4210089663");
	// Google Test Id
	GEngine::AdManager::SetRewardAdId("ca-app-pub-3940256099942544/5224354917");

	//GEngine::AdManager::SetRewardAdId("ca-app-pub-7573801306023183/6644681314");

#endif
	GEngine::AdManager::LoadRewardAd([]() {GE_LOG_DEBUG("AD LOADED"); });
#endif


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
