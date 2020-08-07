#include "gepch.h"
#include "Public/Core/Platform/Window/Mobile/Mobile_Input.h"
#ifdef GE_MOBILE_APP
#include "Public/Core/Platform/Window/Mobile/MobileWindow.h"
#endif
#include "Public/Core/Application/Application.h"

namespace GEngine {

	std::function<void()> Mobile_Input_Callback::showKeyboardFunc;
	std::function<void()> Mobile_Input_Callback::hideKeyboardFunc;
	std::function<void(std::string)> Mobile_Input_Callback::setKeyboardTextFunc;
	std::function<std::string()> Mobile_Input_Callback::getKeyboardTextFunc;
	std::function<double()> Mobile_Input_Callback::getTimeFunc;
    std::function<ViewContext()> Mobile_Input_Callback::getViewContext;
	bool Mobile_Input_Callback::keyboardOpen = false;
	bool Mobile_Input_Callback::touchDown = false;
	int16_t Mobile_Input_Callback::xPos = -1;
	int16_t Mobile_Input_Callback::yPos = -1;
	std::mutex Mobile_Input_Callback::touchMutex;
	std::unordered_map<uint64_t, FTouchInfo> Mobile_Input_Callback::touches;


	bool Mobile_Input::IsKeyPressedImpl(int keycode)
	{
		return false;
	}

	bool Mobile_Input::IsMouseButtonPressedImpl(uint64_t button)
	{
        std::unordered_map<uint64_t, FTouchInfo> t = Mobile_Input_Callback::GetTouches();
        return t.find(button) != t.end();
	}

	float Mobile_Input::GetMouseXImpl(uint64_t id)
	{
		std::unordered_map<uint64_t, FTouchInfo> t = Mobile_Input_Callback::GetTouches();
        if (t.find(id) != t.end()) {
            return t[id].x;
        }
        return -1;
	}

	float Mobile_Input::GetMouseYImpl(uint64_t id)
	{
        std::unordered_map<uint64_t, FTouchInfo> t = Mobile_Input_Callback::GetTouches();
        if (t.find(id) != t.end()) {
            return t[id].y;
        }
		return -1;
	}

	std::pair<float, float> Mobile_Input::GetMousePositionImpl(uint64_t id)
	{
        std::unordered_map<uint64_t, FTouchInfo> t = Mobile_Input_Callback::GetTouches();
           if (t.find(id) != t.end()) {
               return {t[id].x, t[id].y};
           }
        return std::pair<float,float>(-1.0f,-1.0f);
	}



void Mobile_Input_Callback::Touched(uint64_t id, int state, float x, float y, float force)
{
	if (GEngine::Application::GetApp()->GetWindow() != nullptr) {
		/*@TODO ABSTRACT OUT A MOBILE WINDOW CLASS*/
#ifdef GE_MOBILE_APP
		GEngine::MobileWindow* mb = static_cast<GEngine::MobileWindow*>(GEngine::Application::GetApp()->GetWindow());
        if (mb) {
			switch (state) {
				// Touch begin
				case 0: {
                    std::lock_guard<std::mutex> lock(touchMutex);
                    FTouchInfo info = {id, state, x, y,force};
                    Mobile_Input_Callback::touches[id] = std::move(info);
					break;
				}
				// Touch move
				case 1: {
                    std::lock_guard<std::mutex> lock(touchMutex);
                    Mobile_Input_Callback::touches[id].state = state;
                    Mobile_Input_Callback::touches[id].x = x;
                    Mobile_Input_Callback::touches[id].y = y;
					break;
				}
				// Touch end
				case 2: {
                    std::lock_guard<std::mutex> lock(touchMutex);
                    Mobile_Input_Callback::touches.erase(id);
                    break;
				}
				// Touch Cancel
				case 3: {
					std::lock_guard<std::mutex> lock(touchMutex);
					Mobile_Input_Callback::touches.erase(id);
					break;
				}
            }
			mb->TouchEvent(id, state, x, y);

		}
#endif
	}
}



}
