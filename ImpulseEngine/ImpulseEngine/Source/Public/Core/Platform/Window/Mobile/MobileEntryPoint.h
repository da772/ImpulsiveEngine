#pragma once

#include "GEngine.h"
#include "Public/Core/Core.h"
#include "Public/Core/Platform/Window/Mobile/MobileWindow.h"

extern GEngine::Application* GEngine::CreateApplication();

class MobileEntry {
    
public:
    static GEngine::Application* App;

};

static inline void App_Update() {
    if (MobileEntry::App == nullptr) return;
   
    MobileEntry::App->Update();
}

static inline void App_Draw() {
    if (MobileEntry::App == nullptr) return;
    MobileEntry::App->Draw();
}

static inline void App_Create(int width, int height) {
    
    if (MobileEntry::App == nullptr) {
       
        GEngine::Log::Init();
        GEngine::ThreadPool::Setup();
        GEngine::Networking::Init();
        MobileEntry::App = GEngine::CreateApplication();
        MobileEntry::App->Setup();
        GE_CORE_DEBUG("Window Size: {0}, {1}", width, height);
        MobileEntry::App->GetWindow()->SetSize(width, height);
    }
    else {
        GE_CORE_DEBUG("CREATED APP: {0}", MobileEntry::App == nullptr);
    }
}

static inline void App_Start() {
    if (MobileEntry::App == nullptr) return;
    GEngine::Application::Start();
}

static inline void App_Stop() {
	if (MobileEntry::App == nullptr) return;
	GEngine::Application::Stop();
}

static inline void App_Resize(int width, int height) {
    if (MobileEntry::App == nullptr) return;
    GE_CORE_DEBUG("Window ReSize: {0}, {1}", width, height);
    MobileEntry::App->GetWindow()->SetSize(width, height);
}

static inline void App_UnloadGraphics() {
    GE_CORE_DEBUG("UNLOADING GRAPHICS");
    GEngine::Application::UnloadGraphics();
}

static inline void App_ReloadGraphics() {
    GE_CORE_DEBUG("RELOADING GRAPHICS");
    GEngine::Application::ReloadGraphics();
}

static inline void App_Shutdown() {
    MobileEntry::App->Shutdown();
    GE_CORE_DEBUG("APP DELETED");
    GEngine::Networking::Shutdown();
    GEngine::ThreadPool::Shutdown();
    delete MobileEntry::App;
    MobileEntry::App = nullptr;
}
