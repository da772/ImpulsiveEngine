#include "gepch.h"
#include "Public/Core/Platform/Window/Mobile/MobileWindow.h"
#include "Public/Core/Renderer/RenderCommand.h"
#include "Public/Core/Renderer/Renderer.h"
#include "Public/Core/Events/ApplicationEvent.h"
#include "Public/Core/Events/TouchEvent.h"
#include "Public/Core/Platform/Window/Mobile/Mobile_Input.h"

namespace GEngine {
    
        
        MobileWindow::MobileWindow(const WindowData& props)
        {
            
        }
        
        MobileWindow::~MobileWindow()
        {
            Shutdown();
        }
        
        void MobileWindow::Init(const WindowData& props)
        {
            m_Data.Title = props.Title;
            m_Data.Width = props.Width;
            m_Data.Height = props.Height;
            
            GE_CORE_INFO("MOBILE API: Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);
            
            m_Context = Scope<GraphicsContext>(GraphicsContext::Create(nullptr));
             
            
        }
        
		void MobileWindow::TouchEvent(uint64_t id, uint16_t state, int16_t x, int16_t y)
		{

			switch (state)
			{
            // Touch down
			case 0:
			{
				TouchPressedEvent event(id, x,y);
				m_Data.EventCallback(event);
				break;
			}
            // Touch held down
			case 1:
			{
                TouchHeldEvent event(id, x,y);
                m_Data.EventCallback(event);
				break;
			}
            // Touch released
            case 2:
            {
                TouchReleasedEvent event(id, x,y);
                m_Data.EventCallback(event);
                break;
            }
			}
            //GE_CORE_DEBUG("{0}: {1}, {2}, {3}", id, state, x, y);
            
		}

		void MobileWindow::ShowKeyboard()
		{
            Mobile_Input_Callback::ShowKeyboard();
		}

		std::string MobileWindow::GetKeyboardString() const
		{
            return Mobile_Input_Callback::GetKeyboardValue();
		}

		void MobileWindow::SetKeyboardString(const std::string& text)
		{
            Mobile_Input_Callback::SetKeyboardValue(text);
		}

		void MobileWindow::HideKeyboard() 
		{
            Mobile_Input_Callback::HideKeyboard();
		}

		void MobileWindow::Shutdown()
        {
        }
        
        float const MobileWindow::GetTime() const
        {
            return Mobile_Input_Callback::GetTime();
        }
        
        void MobileWindow::OnUpdate(bool m_Minimized)
        {
            if (!m_Minimized)
                m_Context->SwapBuffers();
            
        }
        
        void MobileWindow::GetFrameBufferSize(int* width, int* height) {
            *width = m_Data.Width;
            *height = m_Data.Height;
        }
        
        void MobileWindow::SetTitle(const std::string& title)
        {

        }
        
        void MobileWindow::SetSize(uint16_t width, uint16_t height)
        {
            m_Data.Width = width;
            m_Data.Height = height;

			WindowResizeEvent event(width, height);
            m_Data.EventCallback(event);
        }
        
        void MobileWindow::SetVSync(bool enabled)
        {
            m_Context->SetVSync(enabled);
            m_Data.VSync = enabled;
        }
        
        bool MobileWindow::IsVSync() const
        {
            return m_Data.VSync;
        }
        
    }


