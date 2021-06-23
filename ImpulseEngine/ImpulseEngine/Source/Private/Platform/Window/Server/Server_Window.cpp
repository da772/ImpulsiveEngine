#include "gepch.h"
#include "Public/Platform/Window/Server/Server_Window.h"

namespace GEngine {

        ServerWindow::ServerWindow(const WindowData& props) {
            
        }
        
        ServerWindow::~ServerWindow() {

        };
        
        void ServerWindow::OnUpdate(bool m_Minimized) {

        };
        
        void ServerWindow::GetFrameBufferSize(int* width, int* height) {

        };
        void ServerWindow::SetTitle(const std::string& title) {

        };
        void ServerWindow::SetSize(uint16_t width, uint16_t height) {

        };
        
        void ServerWindow::SetVSync(bool enabled) {

        };
        bool ServerWindow::IsVSync() const {
            return false;
        };
        void ServerWindow::Shutdown() {

        };
        float const ServerWindow::GetTime() const {
            return 0.f;
        };
        
		void ServerWindow::SetWindowHint(int hint, int value)
		{

		}

		void ServerWindow::Init(const WindowData& props) {
            m_Data.Title = props.Title;
            m_Data.Width = props.Width;
            m_Data.Height = props.Height;

            GE_CORE_INFO("Server API: Creating window: {0} ({1}, {2})", props.Title, props.Width, props.Height);
            
            m_Context = Scope<GraphicsContext>(GraphicsContext::Create(nullptr));

        }

        void ServerWindow::GetSafeArea(int* top, int* bottom, int* left, int* right) {

        };


}