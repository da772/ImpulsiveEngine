#include "gepch.h"

#include "Public/Core/Application/Application.h"

#include "Public/Core/Application/Log.h"
#include "Public/Core/Application/Input.h"

#include "Public/Core/Application/SceneManager.h"
#include "Public/Core/Util/Timestep.h"
#include "Public/Core/Util/ThreadPool.h"
#include "Public/Core/Util/Time.h"

#include "Public/Core/Renderer/Pipeline/RenderPipeline.h"
#include "Public/Core/Collision/CollisionDetection.h"

#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Renderer/Graphics/Font.h"

#include "Public/Core/Ads/AdManager.h"

#include "Public/Core/Physics/Physics.h"
#include "Public/Platform/Window/Mobile/Mobile_Input.h"

#include "Public/Core/Audio/AudioManager.h"

#include "Public/Core/Util/Utility.h"

#include "Public/Core/Networking/Networking.h"

#include "Public/Platform/Scripting/ScriptApi.h"

/*DEBUG*/



namespace GEngine {
    
    Application* Application::s_Instance = nullptr;
    FWindowApi Application::s_windowApi = FWindowApi::NONE;
    FGraphicsApi Application::s_graphicsApi = FGraphicsApi::NONE;
    bool Application::s_debugTools = false;

    Application::Application() {
        GE_CORE_ASSERT(!Application::s_Instance, "Application Already Exists")
        Application::s_Instance = this;
		//SetGraphicsApi(GetDefaultGraphicsApi());
		//SetWindowApi(GetDefaultWindowApi());        
    }

	Application::~Application()
	{
		
	}

	Application::Application(const char* title, int width, int height) : m_width (width) , m_height(height)
	{
		GE_CORE_ASSERT(!Application::s_Instance, "Application Already Exists")
		Application::s_Instance = this;
        this->title = title;
        
		SetGraphicsApi(GetDefaultGraphicsApi());
		SetWindowApi(GetDefaultWindowApi());
        
	}

    void Application::LayerSetup() {
        if (!m_ImGuiLayer && b_EnableImGui)
            m_ImGuiLayer = new ImGuiLayer();
        if (m_ImGuiLayer)
            PushOverlay(m_ImGuiLayer);
    }
    
    void Application::LayerReset() {
        if (m_ImGuiLayer) {
            PopOverlay(m_ImGuiLayer);
            delete m_ImGuiLayer;
            m_ImGuiLayer = nullptr;
        }
    }
    
    void Application::CleanDirtyApi()
    {
        Input::Flush();
        bool bCleaned = false;
        if (b_NewWindowApi) {
            SetWindowApi(tempWindowApi);
            b_NewWindowApi = false;
            bCleaned = true;
        }
        
        if (b_NewGraphicsApi) {
            SetGraphicsApi(tempGraphicsApi);
            b_NewGraphicsApi = false;
            bCleaned = true;
        }
        
        if (bCleaned) {
            OnCleanDirtyApi();
        }
        
    }

    void Application::UnloadGraphics() {
        if (Application::GetApp() == nullptr || !Application::GetApp()->m_Running) return;
        if (!AdManager::AdPlaying() && Application::GetApp()->m_loaded) Application::GetApp()->Pause();
        CollisionDetection::Reset();
        SceneManager::UnloadGraphics();
        Renderer::Unload();
        Font::UnloadGraphics();
        Texture2D::UnloadTextures();
        Shader::UnloadShaders();
        if (!AdManager::AdPlaying()) AudioManager::Pause();
        
    }

    void Application::ReloadGraphics() {
        if (Application::GetApp() == nullptr || !Application::GetApp()->m_Running || Application::GetApp()->m_loaded) return;
        if (!AdManager::AdPlaying() && !Application::GetApp()->m_loaded) Application::GetApp()->Resume();
        RenderCommand::Init();
        Texture2D::ReloadTextures();
        Font::ReloadGraphics();
        Shader::ReloadShaders();
        Renderer::Reload();
        if (!AdManager::AdPlaying()) AudioManager::Resume();
        SceneManager::ReloadGraphics();
        
    }
    
	void Application::Stop()
	{
        if (!Application::GetApp()->m_loaded) return;
		Application::GetApp()->Pause();
		AudioManager::Pause();
	}

	void Application::Start()
	{
        if (Application::GetApp()->m_loaded) return;
		Application::GetApp()->Resume();
        AudioManager::Resume();
	}

	int Application::GetWidth()
	{
		if (s_debugTools)
			return s_Instance->m_viewPortWidth;
		if (!s_Instance->GetWindow())
			return s_Instance->m_width;
        int w,h;
        s_Instance->GetWindow()->GetFrameBufferSize(&w, &h);
        return w;
	}

	int Application::GetHeight()
	{
		if (s_debugTools)
			return s_Instance->m_viewPortHeight;
        if (!s_Instance->GetWindow())
            return s_Instance->m_height;
        int w,h;
        s_Instance->GetWindow()->GetFrameBufferSize(&w, &h);
        return h;
	}

	int Application::GetWindowWidth()
	{
        return  s_Instance->GetWindow()->GetWindowData().Width;
	}

	int Application::GetWindowHeight()
	{
        return  s_Instance->GetWindow()->GetWindowData().Height;
	}

	int Application::GetSafeTop()
	{
        return  s_Instance->GetWindow()->GetWindowData().safe_top;
	}

	int Application::GetSafeBottom()
	{
        return s_Instance->GetWindow()->GetWindowData().safe_bottom;
	}

	int Application::GetSafeLeft()
	{
        return s_Instance->GetWindow()->GetWindowData().safe_left;
	}

	int Application::GetSafeRight()
	{
        return s_Instance->GetWindow()->GetWindowData().safe_right;
	}

	float Application::GetSafeTopUI()
	{
        return  s_Instance->GetWindow()->GetWindowData().GetSafeTopUI();
	}

	float Application::GetSafeBottomUI()
	{
        return  s_Instance->GetWindow()->GetWindowData().GetSafeBottomUI();
	}

	float Application::GetSafeLeftUI()
	{
        return s_Instance->GetWindow()->GetWindowData().GetSafeLeftUI();
	}

	float Application::GetSafeRightUI()
	{
        return  s_Instance->GetWindow()->GetWindowData().GetSafeRightUI();
	}

	void Application::QueueWindowApi(const FWindowApi& windowApi) {
        tempWindowApi = windowApi;
        b_NewWindowApi = true;
    }
    
    void Application::QueueGraphicsApi(const FGraphicsApi& graphicsApi)
    {
        tempGraphicsApi = graphicsApi;
        b_NewGraphicsApi = true;
    }
    
	void Application::SetTargetCamera(Camera* camera)
	{
        m_Camera = camera;
        Renderer::SetMainCamera(m_Camera);
	}

	void Application::SetWindowApi(const FWindowApi& windowApi) {
        Application::s_windowApi = windowApi;
        LayerReset();
        

        if (m_Window) {
            m_Window->GetFrameBufferSize(&m_width, &m_height);
        }
        
        m_Window.reset();
        m_Window = Scope<Window>(Create_Window(WindowData(std::string(title), m_width, m_height)));
        m_Window->SetEventCallback(BIND_EVENT_FN(Application, OnEvent));
        LayerSetup();
    }
    
    void Application::SetGraphicsApi(const FGraphicsApi& graphicsApi)
    {
        Application::s_graphicsApi = graphicsApi;
        GraphicsContext::SetGraphicsApi(graphicsApi);
    }
    
	GEngine::WindowApi::FWindowApi Application::GetDefaultWindowApi()
	{
		return GE_WINDOW_API_DEFAULT;
	}

	GEngine::GraphicsApi::FGraphicsApi Application::GetDefaultGraphicsApi()
	{
		return GE_GRAPHICS_API_DEFAULT;
	}

	void Application::EnableImGui(bool b)
	{
        b_EnableImGui = b;
        LayerReset();
        LayerSetup();
	}

	bool Application::InputEnabled()
	{
        return Application::s_Instance->m_enableInput;
	}

	void Application::SetInputEnabled(bool b)
	{
        Application::s_Instance->m_enableInput = b;
	}

	std::string Application::AddOnGamePauseCallback(std::function<void(bool)> f)
	{
        std::string s = Utility::GenerateHash(7);
        while (m_onPausedCallbacks.find(s) != m_onPausedCallbacks.end())
            s = Utility::GenerateHash(7);
        m_onPausedCallbacks[s] = f;
        return s;
	}

	void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }
    
	void Application::PopLayer(Layer* layer)
	{
        m_LayerStack.PopLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }
    
    void Application::PopOverlay(Layer* layer) {
        m_LayerStack.PopOverlay(layer);
        layer->OnDetach();
    }
    
    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application, OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application, OnWindowResize));
        
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
            break;
        }
        Input::ProcessEvents(e);
        SceneManager::OnEvent(e);
    }
    
	void Application::Setup()
	{
		Log::Init();
		ThreadPool::Setup();
		Networking::Init();
        AudioManager::Initialize();
        Physics::Initialize();
        ScriptApi::Initialize();
        SceneManager::Begin();
	}

	void Application::Shutdown()
	{
        m_Running = false;
        CollisionDetection::Reset();
        Texture2D::GetLoadedTexturesRaw().clear();
        Physics::Shutdown();
        SceneManager::Shutdown();
        AdManager::Shutdown();
        AudioManager::Shutdown();
        ScriptApi::Shutdown();
		Networking::Shutdown();
		ThreadPool::Shutdown();
        Application::s_Instance = nullptr;
	}

    void Application::Pause() {
            m_loaded = false;
            if (!Application::GetApp()->m_pause) ThreadPool::PauseThreads();
    }

    void Application::Resume() {
        m_loaded = true;
        if (!Application::GetApp()->m_pause) ThreadPool::UnpauseThreads();
        m_LastFrameTime = Time::GetEpochTimeMS();
    }

	void Application::PauseGame()
	{
        Application::GetApp()->m_pause = true;
		for (auto f : Application::GetApp()->m_onPausedCallbacks) {
			f.second(true);
		}
        ThreadPool::PauseThreads();
        AudioManager::Pause();
	}

	void Application::ResumeGame()
	{
        Application::GetApp()->m_pause = false;
		for (auto f : Application::GetApp()->m_onPausedCallbacks) {
			f.second(false);
		}
        ThreadPool::UnpauseThreads();
        AudioManager::Resume();
	}

	void Application::Update(float ts) {
        
        if (!m_loaded) return;

		GE_PROFILE_TIMER("Application:Run", &profile["Run"]);
		double time = Time::GetEpochTimeMS();
		double timestep = (time - m_LastFrameTime)/1e3f;
        if (m_LastFrameTime == 0)
            timestep = 0;
        profile["FrameTime"] = timestep;
        m_LastFrameTime = time;
		timer += timestep;
        m_frameCount++;
        if (timer >= 1) {
            timer = 0;
            m_fps = m_frameCount;
            m_frameCount = 0;
            profile["FPS"] = m_fps;
        }
		{
			GE_PROFILE_TIMER("Application:OnRender", &profile["OnRender"]);
			m_Window->OnUpdate(false);
		}
		

		{
			GE_PROFILE_TIMER("Application:Layers", &profile["Layers"]);
			
			if (!m_Minimized) {
				for (Layer* layer : m_LayerStack) {
					layer->OnUpdate(timestep);
				}
			}
			
		}

        {
            {
                GE_PROFILE_TIMER("Application:OnUpdate", &profile["OnUpdate"]);



                if (!m_Minimized && !m_pause) {
                    OnUpdate(timestep);
                    SceneManager::Update(timestep);
                }

                {
                    std::queue<std::function<void()>>& f = ThreadPool::GetMainThreadFunctions();
                    if (!f.empty()) {
                        std::mutex& mut = ThreadPool::GetMainFunctionsMutex();
                        std::lock_guard<std::mutex> guard(mut);
                        while (!f.empty()) {
                            f.front()();
                            f.pop();
                        }
                    }
                }
                {
                    std::queue<std::function<void()>>& f = ThreadPool::GetEndThreadFunction();
                    if (!f.empty()) {
                        std::mutex& mut = ThreadPool::GetEndThreadFunctionsMutex();
                        std::lock_guard<std::mutex> guard(mut);
                        while (!f.empty()) {
                            f.front()();
                            f.pop();
                        }
                    }
                }

            }
        }
            {
                if (!m_pause)
                    AudioManager::Update();
            }

			{
				GE_PROFILE_TIMER("Application:PhysicsUpdate", &profile["Physics"]);
				{
					if (!m_pause) {
						Physics::Update(timestep);
						CollisionDetection::CheckCollision();
					}
				}
			}
    }
    
    void Application::Draw() {
        if (!m_loaded) return;
        {
            GE_PROFILE_TIMER("Application:Draw", &profile["Draw"]);
            Renderer::BeginScene(m_Camera);
            {
                GE_PROFILE_TIMER("Application:Render", &profile["Render"]);
                Renderer::Render();
            }
            if (b_EnableImGui) m_ImGuiLayer->Begin();
            size_t size = m_LayerStack.GetSize();
            {
                GE_PROFILE_TIMER("Application:LayerDraw", &profile["LayerDraw"]);
                for (int i = 0; i < size; i++) {
                    if (i >= m_LayerStack.GetSize())
                        break;
                    Layer* layer = *(m_LayerStack.begin() + i);
                    layer->OnDraw();
                    if (b_EnableImGui) layer->OnImGuiRender();
                }
            }
            if (SceneManager::GetCurrentScene() && b_EnableImGui) SceneManager::ImGuiRender();
            if (b_EnableImGui) m_ImGuiLayer->End();
            Renderer::EndScene();
        }
        CleanDirtyApi();
    }
    
    void Application::Run()
    {
        while (m_Running)
        {
            GE_PROFILE_FUNCTION();
            {
                {
                    GE_PROFILE_TIMER("Application:OnUpdate", &profile["TotalFrame"]);
                    Update();
                    Draw();
                }
            }
        }
        
    }
    
    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        if (Application::GetApp()->m_pause) ResumeGame();
        UnloadGraphics();
        return true;
    }
    
    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.GetWidth() == 0 || e.GetHeight() == 0) {
            m_Minimized = false;
            return false;
        }
        
        int width = 0, height = 0;
        m_Window->GetFrameBufferSize(&width, &height);
        Renderer::OnWindowResize(width, height);
        
        m_Minimized = false;
        return false;
    }
    
}
