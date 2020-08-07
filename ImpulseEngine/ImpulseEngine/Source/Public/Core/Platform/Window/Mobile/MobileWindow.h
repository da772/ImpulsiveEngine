#pragma once
#include "Public/Core/Application/Window.h"

class GraphicsContext;

namespace GEngine {
    
    class MobileWindow : public Window
    {
    public:
        MobileWindow(const WindowData& props);
        
        virtual ~MobileWindow();
        
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
        
        virtual void Init(const WindowData& props) override;
        
        virtual void TouchEvent(uint64_t id, uint16_t state, int16_t x, int16_t y);

        virtual void ShowKeyboard();

        virtual std::string GetKeyboardString() const;

        virtual void SetKeyboardString(const std::string& text);

        virtual void HideKeyboard();


    private:
        WindowData m_Data;
        
        
    };
    
}
