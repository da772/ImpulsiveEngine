#include "gepch.h"

#include "Public/Core/Application/Application.h"

#include "Public/Core/Application/Log.h"
#include "Public/Core/Application/Input.h"

#include "Public/Core/Application/SceneManager.h"
#include "Public/Core/Util/Timestep.h"
#include "Public/Core/Util/ThreadPool.h"
#include "Public/Core/Util/Time.h"

#include "Public/Core/Renderer/Pipeline/RenderPipeline.h"
#include "Public/Core/Scripting/ScriptManager.h"
#include "Public/Core/Collision/CollisionDetection.h"

#include "Public/Core/Renderer/Graphics/Shader.h"
#include "Public/Core/Renderer/Graphics/Texture.h"
#include "Public/Core/Renderer/Graphics/Font.h"

#include "Public/Core/Ads/AdManager.h"

#include "Public/Core/Physics/Physics.h"
#include "Public/Core/Platform/Window/Mobile/Mobile_Input.h"



namespace GEngine {



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
        if (Application::GetApp() == nullptr || !Application::GetApp()->m_Running || !Application::GetApp()->m_loaded) return;
        Application::GetApp()->Pause();
        SceneManager::UnloadGraphics();
        Font::UnloadGraphics();
        Texture2D::UnloadTextures();
        Shader::UnloadShaders();
        
        
    }

    void Application::ReloadGraphics() {
        if (Application::GetApp() == nullptr || !Application::GetApp()->m_Running || Application::GetApp()->m_loaded) return;
        Application::GetApp()->Resume();
        RenderCommand::Init();
        Texture2D::ReloadTextures();
        Font::ReloadGraphics();
        Shader::ReloadShaders();
        SceneManager::ReloadGraphics();
    }
    
	int Application::GetWidth()
	{
        return  s_Instance->GetWindow()->GetWindowData().Width;
	}

	int Application::GetHeight()
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
    
    void Application::SetWindowApi(const FWindowApi& windowApi) {
        Application::s_windowApi = windowApi;
        LayerReset();
        

        if (m_Window) {
            m_width = m_Window->GetWidth();
            m_height = m_Window->GetHeight();
        }
        
        m_Window.reset();
        m_Window = Scope<Window>(Window::Create(WindowData(std::string(title), m_width, m_height)));
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

	void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
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
        Physics::Initalize();
        AdManager::Initialize();
        SceneManager::Begin();
	}

	void Application::Shutdown()
	{
        m_Running = false;
        Physics::Shutdown();
        SceneManager::End();
        AdManager::Shutdown();
        Application::s_Instance = nullptr;
	}

    void Application::Pause() {
            m_loaded = false;
    }

    void Application::Resume() {
        m_loaded = true;
        m_LastFrameTime = Time::GetEpochTimeMS();
    }

	void Application::Update(float ts) {
        
        if (!m_loaded) return;

		GE_PROFILE_TIMER("Application:Run", &profile["Run"]);
		double time = Time::GetEpochTimeMS();
		double timestep = (time - m_LastFrameTime)/1e3f;
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
				if (!m_Minimized) {
					OnUpdate(timestep);
                    SceneManager::Update(timestep);
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
                Physics::Update(timestep);
                CollisionDetection::CheckCollision();
            }
			
			
		}
	
		CleanDirtyApi();
    }
    
    void Application::Draw() {
        if (!m_loaded) return;
        Renderer::BeginScene(m_Camera);
        Renderer::Render();
        if (b_EnableImGui) m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack) {
                layer->OnDraw();
                if (b_EnableImGui) layer->OnImGuiRender();
		}
        if (b_EnableImGui) SceneManager::ImGuiRender();
        if (b_EnableImGui) m_ImGuiLayer->End();
        Renderer::EndScene();

       
        
    }
    
    void Application::Run()
    {
        while (m_Running)
        {
            GE_PROFILE_FUNCTION();
            {
                {
                    GE_PROFILE_TIMER("Application:OnUpdate", &profile["FrameTime"]);
                    Update();
                    Draw();
                }
            }
        }
        
    }
    
    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
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
