#pragma once
#include "Core/Application/Window.h"

namespace GEngine {

	typedef void* HANDLE;

	class WIN32_Window : public Window {
	public:
		WIN32_Window(const WindowData& props);

		virtual ~WIN32_Window();

		void OnUpdate(bool m_Minmized) override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
        virtual void GetFrameBufferSize(int* width, int* height) override {};

		virtual void SetTitle(const std::string& title) override {};
		virtual void SetSize(uint16_t width, uint16_t height) override {};

		// Window attributes
		virtual void Init(const WindowData& props) override;
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		inline virtual void* GetNativeWindow() const { return m_Window; }
		bool IsVSync() const override;
		virtual void Shutdown() override;
		inline virtual const float GetTime() const override { return 0.f; }



		void GetSafeArea(int* top, int* bottom, int* left, int* right) override;

	private:
		HANDLE m_Window;
		WindowData m_Data;


	};



}
