#include <EntryPoint.h>
#include "cr.h"
#include "WizardJump.h"

GEngine::Application* app = nullptr;
bool bUnloaded = false;

CR_EXPORT int cr_main(struct cr_plugin* ctx, enum cr_op operation) {
	assert(ctx);

	switch (operation) {
	case CR_LOAD: {
		
		if (app == nullptr) {
			GEngine::Log::Init();
			GEngine::ThreadPool::Setup();
			GEngine::Networking::Init();
			app = GEngine::CreateApplication();
			app->Setup();
		}
		GE_CORE_DEBUG("RELOAD: Loading App v{0}", ctx->version);
	
		GEngine::FileSystem::PakDirectory(GEngine::FileSystem::GetParentExecuteableDir(3) + "WizardJump/Content",
			GEngine::FileSystem::FilePath("Data/Content.pak"), false);
		GEngine::FileSystem::LoadPak("Data/Content.pak");
		GEngine::FileSystem::Copy(GEngine::FileSystem::FilePath("Data/Content.pak"),
			GEngine::FileSystem::GetParentExecuteableDir(3) + "WizardJump/Data/Content.pak", false);
		GE_LOG_DEBUG("Refreshing Pak File");
		GEngine::SceneManager::SetCurrentScene("splashScreen");
		return 0;
	}
	case CR_UNLOAD: {
		GE_CORE_DEBUG("RELOAD: Unload App v{0} to v{1}", ctx->version, ctx->next_version);
		app->Shutdown();
		GEngine::Networking::Shutdown();
		GEngine::ThreadPool::Shutdown();
		delete app;
		app = nullptr;
		GEngine::ThreadPool::Setup();
		GEngine::Networking::Init();
		app = GEngine::CreateApplication();
		app->Setup();
		return 0;
	}
	case CR_CLOSE: {
		GE_CORE_DEBUG("CLOSE APP");
		app->Shutdown();
		GEngine::Networking::Shutdown();
		GEngine::ThreadPool::Shutdown();
		delete app;
		app = nullptr;
		return 0; // the plugin will close and not reload anymore
	}
	}

	if (app) {
		app->Update();
		app->Draw();
		//GE_CORE_DEBUG("UPDATE COMPLETE!");
	}

	return 0;
}


GEngine::Application* GEngine::CreateApplication()
{
	return new WizardJump();
}
