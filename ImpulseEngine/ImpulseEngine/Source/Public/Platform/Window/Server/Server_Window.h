#pragma once
#include "Public/Core/Application/Window.h"

class GraphicsContext;

namespace GEngine {
    
    class ServerWindow : public Window
    {
    public:
        ServerWindow(const WindowData& props);
        
        virtual ~ServerWindow();
        
        void OnUpdate(bool m_Minimized) override;
        
        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }
        virtual void GetFrameBufferSize(int* width, int* height) override;
        virtual void SetTitle(const std::string& title) override;
        virtual void SetSize(uint16_t width, uint16_t height) override;
        
        // Window attributes
        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        inline virtual void* GetNativeWindow() const override { return nullptr; };
        bool IsVSync() const override;
        virtual void Shutdown() override;
        virtual float const GetTime() const override;
        virtual void SetWindowHint(int hint, int value) override;
        
        virtual void Init(const WindowData& props) override;
        virtual void GetSafeArea(int* top, int* bottom, int* left, int* right) override;

    private:
        
        
    };
    
}
