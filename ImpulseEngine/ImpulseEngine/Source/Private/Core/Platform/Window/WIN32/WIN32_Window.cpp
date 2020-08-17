#include "gepch.h"
#include "Core/Platform/Window/WIN32/WIN32_Window.h"

#ifdef GE_WINDOW_API_WIN32
#include <windows.h>

namespace GEngine {

	WIN32_Window::WIN32_Window(const WindowData& props)
	{

		const wchar_t CLASS_NAME[] = L"Sample Window Class";

		WNDCLASS wc = { };

		wc.lpszClassName = CLASS_NAME;

		RegisterClass(&wc);

		HWND hwnd = CreateWindowEx(
			0,                              // Optional window styles.
			CLASS_NAME,                     // Window class
			L"Learn to Program Windows",    // Window text
			WS_OVERLAPPEDWINDOW,            // Window style

			// Size and position
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

			NULL,       // Parent window    
			NULL,       // Menu
			NULL,  // Instance handle
			NULL        // Additional application data
		);

	}

	WIN32_Window::~WIN32_Window()
	{

	}

	void WIN32_Window::OnUpdate(bool m_Minmized)
	{

	}

	void WIN32_Window::SetVSync(bool enabled)
	{

	}

	bool WIN32_Window::IsVSync() const
	{
		return false;
	}

	void WIN32_Window::Shutdown()
	{

	}

	void WIN32_Window::GetSafeArea(int* top, int* bottom, int* left, int* right)
	{
		
	}

	void WIN32_Window::Init(const WindowData& props)
	{

	}

}

#endif
