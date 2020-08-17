#pragma once
#include "Public/Core/Application/Window.h"


struct GLFWwindow;
class GraphicsContext;

namespace GEngine {

	class GLFW_Window : public Window
	{
	public:
		GLFW_Window(const WindowData& props);

		virtual ~GLFW_Window();

		void OnUpdate(bool m_Minimized) override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
        virtual void GetFrameBufferSize(int* width, int* height) override;
		virtual void SetTitle(const std::string& title) override;
		virtual void SetSize(uint16_t width, uint16_t height) override;

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		inline virtual void* GetNativeWindow() const { return m_Window; }
		bool IsVSync() const override;
		virtual void Shutdown() override;
		virtual float const GetTime() const override;


		virtual void Init(const WindowData& props) override;


		void GetSafeArea(int* top, int* bottom, int* left, int* right) override;

	private:
		
		
		//HANDLE myHandle;
	private:
		GLFWwindow* m_Window;
		


	};

}
