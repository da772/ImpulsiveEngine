#include "ImpulseEditor.h"
#include <glm/gtc/matrix_transform.hpp>
#ifdef GE_EDITOR
#include "Editor/Project/ProjectSelectLayer.h"
#endif

#include "Shared/Scene/EditorScene.h"

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
	/*
#ifdef GE_HOT_RELOAD
	ScriptApi::SetBuild_Native(FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET) + "ImpulseEditor/Scripts/CPP/", "Scripts_CPP");
	ScriptApi::SetMake_Native(FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET) + "Build/", "GenerateProject_Scripting");
	ScriptApi::OutputDir_Native(GEngine::FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET) + "ImpulseEditor/Scripts/CPP/Generated/");
	ScriptApi::SetRelativePath_Native("../Scripts/");
	ScriptApi::Load(GEngine::FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET) + "ImpulseEditor/Scripts/CPP/Scripts/", ".h");
#else 
	__ReflectionMap__loadGeneratedFiles(ScriptApi::GetStorage_Native());
#endif
*/
	//GEngine::SceneManager::AddScene<SplashScreenScene>("splashScreen");
#ifdef GE_HOT_RELOAD
	/*
	ScriptApi::SetBuild_Native(FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET) + "ImpulseEditor/NativeScripts/", "NativeScripts");
	ScriptApi::SetMake_Native(FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET) + "Generate/", "GenerateProject_Scripting");
	ScriptApi::OutputDir_Native(GEngine::FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET) + "ImpulseEditor/NativeScripts/Generated/");
	ScriptApi::SetRelativePath_Native("../Scripts/");
	ScriptApi::Load(GEngine::FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET) + "ImpulseEditor/NativeScripts/Scripts/", ".h");
	ScriptApi::GetReflector_Native()->CallStaticFunction<void>("ExampleScript", "TestFunction");
	*/
#else
	__ReflectionMap__loadGeneratedFiles(GEngine::ScriptApi::GetStorage_Native());

#endif
	//GEngine::SceneManager::AddScene<MenuScene>("menuScene");
	//GEngine::SceneManager::SetCurrentScene("menuScene");
	//*/

	GEngine::SceneManager::AddScene<Editor::EditorScene>("EditorScene");
	GEngine::SceneManager::SetCurrentScene("EditorScene", true);
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
	if (event.GetEventType() == GEngine::EventType::WindowResize) {
		if (!GEngine::Application::DebugTools()) {
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

#endif
	
#if defined(GE_EDITOR)
	s_debugTools = true;
	this->m_width = 960;
	this->m_height = 540;
	this->m_viewPortWidth = 1920;
	this->m_viewPortHeight = 1080;

#ifndef GE_PACKAGE
	/*
	GEngine::FileSystem::PakDirectory(GEngine::FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET)+"ImpulseEditor/Engine/EngineContent",
		GEngine::FileSystem::FilePath("Data/EngineContent.pak"), false);

	GEngine::FileSystem::Copy(GEngine::FileSystem::FilePath("Data/EngineContent.pak"), 
		GEngine::FileSystem::GetParentExecuteableDir(GE_PRJ_OFFSET) + "ImpulseEditor/Data/EngineContent.pak", false, false);
	*/
#endif
#endif

#ifdef GE_EDITOR
	GEngine::FileSystem::LoadPak("Data/EditorContent.pak");
#else
	GEngine::FileSystem::LoadPak("Data/" + std::string(this->title) + "Content.pak");
#endif
	//GEngine::FileSystem::LoadPak("Data/EditorContent.pak");
	GEngine::FileSystem::LoadPak("Data/EngineContent.pak");


	GE_LOG_INFO("Cores: {0}, Freq: {1}", GEngine::DeviceInfo::GetCpuCount(), GEngine::DeviceInfo::GetCpuFreq());
	SetRenderScale(1.f);
	SetRenderSamples(0);


	SetGraphicsApi(GetDefaultGraphicsApi());
	SetWindowApi(GetDefaultWindowApi());
	GetWindow()->SetVSync(false);

#if defined(GE_EDITOR)
	EnableImGui(s_debugTools);
#else
	EnableImGui(false);
#endif
	
	/*
	MobileWindow* window = dynamic_cast< GEngine::MobileWindow*>(GetWindow());
	if (window) {
		//window->ShowKeyboard();
	}
	*/
	

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
#ifdef GE_EDITOR
	if (s_debugTools) {
		//m_ExampleLayer = new ExampleLayer();
		//PushLayer(m_ExampleLayer);
		m_ProjectSelectLayer = new Project::ProjectSelectLayer("ProjectSelect");
		PushLayer(m_ProjectSelectLayer);
	}
#else
	m_ExampleLayer = new ExampleLayer();
	PushLayer(m_ExampleLayer);
#endif
}

ImpulseEditor::~ImpulseEditor()
{
#ifdef GE_EDITOR
	fd = nullptr;
#endif
}

void ImpulseEditor::OnCleanDirtyApi()
{
	m_ExampleLayer->OnDetach();
	m_ProjectSelectLayer->OnDetach();
	m_ExampleLayer->OnAttach();
	m_ProjectSelectLayer->OnAttach();
}

void ImpulseEditor::OnImGuiSetup()
{
#ifdef GE_EDITOR
	auto io = ImGui::GetIO();
	fd = GEngine::FileSystem::FileDataFromPath("EngineContent/Fonts/roboto.ttf");
	ImFontConfig cfg = {  };
	cfg.FontDataOwnedByAtlas = false;
	mainFont = io.Fonts->AddFontFromMemoryTTF(fd->GetData(), (int)fd->GetDataSize(), 20, &cfg);
	smallFont = io.Fonts->AddFontFromMemoryTTF(fd->GetData(), (int)fd->GetDataSize(), 17, &cfg);
	largeFont = io.Fonts->AddFontFromMemoryTTF(fd->GetData(), (int)fd->GetDataSize(), 23, &cfg);
	io.Fonts->Build();
#endif
}

void ImpulseEditor::OnUpdate(GEngine::Timestep timeStep)
{

}
