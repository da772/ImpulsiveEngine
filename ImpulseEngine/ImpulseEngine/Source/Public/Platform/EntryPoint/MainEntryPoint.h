#pragma once
#include "GEngine.h"
#include "Public/Core/Core.h"

extern GEngine::Application* GEngine::CreateApplication();
#if defined(GE_CONSOLE_APP) && !defined(GE_HOT_RELOAD)
#if defined(GE_PLATFORM_WINDOWS) && !defined(GE_DEBUG)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char** argv)
#endif
{
	GE_PROFILE_BEGIN_SESSION("Startup", "GEngineProfile-Startup.json");
	GEngine::Log::Init();
	GEngine::ThreadPool::Setup();
	GEngine::Networking::Init();
	auto app = GEngine::CreateApplication();
	app->Setup();
	GE_PROFILE_END_SESSION();

	GE_PROFILE_BEGIN_SESSION("Runtime", "GEngineProfile-Runtime.json");
	app->Run();
	GE_PROFILE_END_SESSION();

	GE_PROFILE_BEGIN_SESSION("Shutdown", "GEngineProfile-Shutdown.json");
	app->Shutdown();
	GEngine::Networking::Shutdown();
	GEngine::ThreadPool::Shutdown();
	delete app;
	GE_PROFILE_END_SESSION();

	return 0;
}
#endif
