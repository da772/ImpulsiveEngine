#include "ImpulseEditor.h"
#include <glm/gtc/matrix_transform.hpp>
#include "SplashScreenScene.h"
#include "MapEditor.hpp"


#include "MainGameScene.h"
#include "MenuScene.h"

#include "Lighting/Lighting.hpp"

#include "Reflection.map.generated.h"



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
#ifdef GE_HOT_RELOAD
	ScriptApi::SetBuild_Native(FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET) + "ImpulseEditor/Scripts/CPP/", "Scripts_CPP");
	ScriptApi::SetMake_Native(FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET) + "Project/", "GenerateProject_Scripting");
	ScriptApi::OutputDir_Native(GEngine::FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET) + "ImpulseEditor/Scripts/CPP/Generated/");
	ScriptApi::SetRelativePath_Native("../Scripts/");
	ScriptApi::Load(GEngine::FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET) + "ImpulseEditor/Scripts/CPP/Scripts/", ".h");
#else 
	__ReflectionMap__loadGeneratedFiles(ScriptApi::GetStorage_Native());
#endif
	//GEngine::SceneManager::AddScene<SplashScreenScene>("splashScreen");
	//GEngine::SceneManager::AddScene<MainGameScene>("mainGame");
	GEngine::SceneManager::AddScene<MenuScene>("menuScene");
	GEngine::SceneManager::SetCurrentScene("menuScene");
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
	if (event.GetEventType() == EventType::WindowResize) {
		if (!Application::DebugTools()) {
			const std::vector<GEngine::FPipeline> pipelines = GEngine::Renderer::GetPipelines();
			for (const GEngine::FPipeline& p : pipelines) {
				p.p->GetFrameBuffer()->UpdateSize(GEngine::Application::GetWidth(), GEngine::Application::GetHeight());
			}
		}
	}


}

ImpulseEditor::ImpulseEditor()
{
    SetUIResolution(1080, 1920);
#ifdef GE_CONSOLE_APP
	this->m_width = 540;
	this->m_height = 960;
	this->title = GE_APP_NAME;
	s_debugTools = false;
#endif
	
#if defined(GE_EDITOR)
	if (s_debugTools) {
		this->m_width = 1280;
		this->m_height = 720;
		this->m_viewPortWidth = 1080;
		this->m_viewPortHeight = 1920;
	}

	GEngine::FileSystem::PakDirectory(GEngine::FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET)+"ImpulseEditor/Content",
		GEngine::FileSystem::FilePath("Data/Content.pak"), false);
#endif
	GEngine::FileSystem::LoadPak("Data/Content.pak");
	GEngine::Ref<GEngine::FileData> manifest = GEngine::FileSystem::LoadFileFromPak("Content/Editor/Manifest.cxml");
#ifdef GE_EDITOR
	GEngine::FileSystem::Copy(GEngine::FileSystem::FilePath("Data/Content.pak"), 
		GEngine::FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET) + "ImpulseEditor/Data/Content.pak", false);
#endif
	GE_LOG_INFO("Cores: {0}, Freq: {1}", DeviceInfo::GetCpuCount(), DeviceInfo::GetCpuFreq());
	SetRenderScale(1.f);
	SetRenderSamples(0);

	SetGraphicsApi(GetDefaultGraphicsApi());
	SetWindowApi(GetDefaultWindowApi());
	GetWindow()->SetVSync(true);

#if defined(GE_EDITOR)
	EnableImGui(s_debugTools);
#else
	EnableImGui(false);
#endif
	
	MobileWindow* window = dynamic_cast< GEngine::MobileWindow*>(GetWindow());
	if (window) {
		//window->ShowKeyboard();
	}
	

#ifdef GE_MOBILE_APP
    GEngine::AdManager::SetTimeout(5.f);
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
#ifdef GE_CONSOLE_APP
	if (s_debugTools) {
		m_DebugLayer = new DebugLayer();
		PushLayer(m_DebugLayer);
	}
#endif
	m_ExampleLayer = new ExampleLayer();
	PushLayer(m_ExampleLayer);

	Lighting::Initialize();
}

void ImpulseEditor::OnCleanDirtyApi()
{
	m_ExampleLayer->OnDetach();
	m_DebugLayer->OnDetach();
	m_ExampleLayer->OnAttach();
	m_DebugLayer->OnAttach();
}

void ImpulseEditor::OnUpdate(GEngine::Timestep timeStep)
{

}