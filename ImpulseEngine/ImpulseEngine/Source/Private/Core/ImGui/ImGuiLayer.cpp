#include "gepch.h"
#include "Public/Core/ImGui/ImGuiLayer.h"
#include "imgui.h"
#include "Public/Core/Application/Application.h"

#ifdef GE_WINDOW_API_GLFW
#include <GLFW/glfw3.h>
#include "examples/imgui_impl_glfw.h"
#endif

#ifdef GE_GRAPHICS_API_OPENGL_3_3
#include "examples/imgui_impl_opengl3.h"
#include <glad/glad.h>
#endif

#ifdef GE_GRAPHICS_API_VULKAN
#include <vulkan/vulkan.h>
#include "examples/imgui_impl_vulkan.h"
#include "Public/Platform/Graphics/Vulkan/Vulkan_GraphicsContext.h"
#endif

#include "Public/Core/FileSystem/FileSystem.h"




namespace GEngine {
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{

	}


	ImGuiLayer::~ImGuiLayer()
	{

	}
	#ifdef GE_CONSOLE_APP
	ImFont* font2;
	#endif
	void ImGuiLayer::OnAttach()
	{
		#ifdef GE_CONSOLE_APP
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;


		ImGui::StyleColorsCustom();

		ImFont* font1 = io.Fonts->AddFontDefault();
		Ref<FileData> fd = FileSystem::FileDataFromPath("EngineContent/Fonts/roboto.ttf");

		void* fnt = malloc(fd->GetDataSize());
		memcpy( fnt, fd->GetData(), fd->GetDataSize());
		font2 = io.Fonts->AddFontFromMemoryTTF(fnt, fd->GetDataSize(), 20);
		//ImGui::PushFont(font2);
		ImGuiStyle& style = ImGui::GetStyle();

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
		ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 0);
		
		
		

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.f;
			style.Colors[ImGuiCol_WindowBg].w = 1.f;
		}

		Api_OnAttach();
		#endif
	}

	
	void ImGuiLayer::OnDetach()
	{
		#ifdef GE_CONSOLE_APP
		Api_OnDetach();
		ImGui::DestroyContext();
		#endif
	}


	void ImGuiLayer::OnImGuiRender()
	{
		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);
	}

	void ImGuiLayer::Begin()
	{
		#ifdef GE_CONSOLE_APP
		Api_Begin();
		ImGui::NewFrame();
		ImGui::PushFont(font2);
		
		#endif
	}

	void ImGuiLayer::End()
	{
		#ifdef GE_CONSOLE_APP
		ImGuiIO& io = ImGui::GetIO();
		Application* app = Application::GetApp();
		io.DisplaySize = ImVec2((float)app->GetWindow()->GetWidth(), (float)app->GetWindow()->GetHeight());
		ImGui::PopFont();
		ImGui::Render();
		
		Api_End();
		#endif
	}

	
	void ImGuiLayer::Api_OnDetach()
	{
		#ifdef GE_CONSOLE_APP
		switch (GraphicsContext::GetGraphicsApi())
		{
		case FGraphicsApi::OPENGL:
			OpenGL_OnDetach();
			break;
		case FGraphicsApi::VULKAN:
			Vulkan_OnDetach();
			break;
		default:
			GE_CORE_ASSERT(false, "ImGui not supported on this platform!");
			break;
		}
		#endif
	}

	void ImGuiLayer::OpenGL_OnDetach()
	{
#ifdef GE_GRAPHICS_API_OPENGL_3_3
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
#endif
	}

	void ImGuiLayer::Vulkan_OnDetach()
	{
#ifdef GE_GRAPHICS_API_VULKAN
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
#endif
	}

	void ImGuiLayer::Api_OnAttach()
	{
		switch (GraphicsContext::GetGraphicsApi())
		{
		case FGraphicsApi::OPENGL:
			OpenGL_OnAttach();
			break;
		case FGraphicsApi::VULKAN:
			Vulkan_OnAttach();
			break;
		default:
			GE_CORE_ASSERT(false, "ImGui not supported on this platform!");
			break;
		}
	}

	void ImGuiLayer::OpenGL_OnAttach()
	{
#ifdef GE_GRAPHICS_API_OPENGL_3_3
		Application* app = Application::GetApp();
		GLFWwindow* window = static_cast<GLFWwindow*>(app->GetApp()->GetWindow()->GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
#endif
	}

	void ImGuiLayer::Vulkan_OnAttach()
	{
#ifdef GE_GRAPHICS_API_VULKAN
		Application* app = Application::GetApp();
		GLFWwindow* window = static_cast<GLFWwindow*>(app->GetApp()->GetWindow()->GetNativeWindow());
		ImGui_ImplGlfw_InitForVulkan(window, true);
		ImGui_ImplVulkan_InitInfo info = { 0 };
		info.Device = Vulkan_GraphicsContext::GetDevice();
		info.PhysicalDevice = Vulkan_GraphicsContext::Get()->physicalDevice;
		info.DescriptorPool = Vulkan_GraphicsContext::Get()->descriptorPool;
		info.Queue = Vulkan_GraphicsContext::Get()->graphicsQueue;
		QueueFamilyIndices queueFamilyIndices = Vulkan_GraphicsContext::Get()->findQueueFamilies(Vulkan_GraphicsContext::Get()->physicalDevice);
		info.QueueFamily = queueFamilyIndices.graphicsFamily.value();
		info.Instance = Vulkan_GraphicsContext::Get()->instance;
		ImGui_ImplVulkan_Init(&info, Vulkan_GraphicsContext::Get()->renderPass);
#endif
		
	}

	void ImGuiLayer::Api_Begin()
	{
		switch (GraphicsContext::GetGraphicsApi())
		{
		case FGraphicsApi::OPENGL:
			OpenGL_Begin();
			break;
		case FGraphicsApi::VULKAN:
			Vulkan_Begin();
			break;
		default:
			GE_CORE_ASSERT(false, "ImGui not supported on this platform!");
			break;
		}
	}

	void ImGuiLayer::OpenGL_Begin()
	{
#ifdef GE_GRAPHICS_API_OPENGL_3_3
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
#endif
	}

	void ImGuiLayer::Vulkan_Begin()
	{
#ifdef GE_GRAPHICS_API_VULKAN
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
#endif
	}

	void ImGuiLayer::Api_End()
	{
		switch (GraphicsContext::GetGraphicsApi())
		{
		case FGraphicsApi::OPENGL:
			OpenGL_End();
			break;
		case FGraphicsApi::VULKAN:
			Vulkan_End();
			break;
		default:
			GE_CORE_ASSERT(false, "ImGui not supported on this platform!");
			break;
		}
	}

	void ImGuiLayer::OpenGL_End()
	{
#ifdef GE_GRAPHICS_API_OPENGL_3_3
		ImGuiIO& io = ImGui::GetIO();
		Application* app = Application::GetApp();
		io.DisplaySize = ImVec2((float)app->GetWindow()->GetWidth(), (float)app->GetWindow()->GetHeight());
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
#endif
	}

	void ImGuiLayer::Vulkan_End()
	{
#ifdef GE_GRAPHICS_API_VULKAN
		ImGuiIO& io = ImGui::GetIO();
		Application* app = Application::GetApp();
		io.DisplaySize = ImVec2((float)app->GetWindow()->GetWidth(), (float)app->GetWindow()->GetHeight());
		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), Vulkan_GraphicsContext::Get()->commandBuffers[Vulkan_GraphicsContext::Get()->imageIndex]);
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
#endif
	}


}
