#include "gepch.h"
#include "Public/Core/Platform/Window/Mobile/Mobile_Input.h"
#ifdef GE_MOBILE_APP
#include "Public/Core/Platform/Window/Mobile/MobileWindow.h"
#endif
#include "Public/Core/Application/Application.h"
#include "Public/Core/Events/TouchEvent.h"
#include "Public/Core/Collision/CollisionDetection.h"
#include "Public/Core/Util/GEMath.h"
#include "Public/Core/Util/Time.h"
#include "Public/Core/Util/ThreadPool.h"

namespace GEngine {

	std::function<void()> Mobile_Input_Callback::showKeyboardFunc;
    std::function<void(int*,int*,int*,int*)> Mobile_Input_Callback::getSafeView;
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

    void Mobile_Input::ClearTouches() {
        Mobile_Input_Callback::ClearTouches();
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



	void Mobile_Input::ProcessEvent(const Event& e)
	{
		int width, height;
		Application::GetApp()->GetWindow()->GetFrameBufferSize(&width, &height);
		if (e.GetEventType() == EventType::TouchPressed) {
			const TouchPressedEvent& event = (TouchPressedEvent&)e;
			
			if (GetTouchCount() <= 1) {
				CollisionDetection::InteractionUI(
					GEMath::MapRange(event.GetX() / (float)width, 0, 1, -1, 1),
					-GEMath::MapRange(event.GetY() / (float)height, 0, 1, -1, 1));
			}
		}

		if (e.GetEventType() == EventType::TouchReleased) {
			const TouchReleasedEvent& event = (TouchReleasedEvent&)e;
			if (GetTouchCount() <= 0) {
				CollisionDetection::InteractionEndUI(
					GEMath::MapRange(event.GetX() / (float)width, 0, 1, -1, 1),
					-GEMath::MapRange(event.GetY() / (float)height, 0, 1, -1, 1));
			}
		}


	}

void Mobile_Input_Callback::Touched(uint64_t id, int state, float x, float y, float force)
{
	if (GEngine::Application::GetApp()->GetWindow() != nullptr) {
		
#ifdef GE_MOBILE_APP
		GEngine::MobileWindow* mb = static_cast<GEngine::MobileWindow*>(GEngine::Application::GetApp()->GetWindow());
        if (mb) {
			GE_CORE_DEBUG("TOUCH: {0} - {1}", id, state);
			switch (state) {
				// Touch begin
				case 0: {
                    std::lock_guard<std::mutex> lock(touchMutex);
					if (Mobile_Input_Callback::touches.find(id) == Mobile_Input_Callback::touches.end()) {
                        FTouchInfo info(id, state, x, y,force, Time::GetEpochTimeNS());
                        Mobile_Input_Callback::touches[id] = std::move(info);
					} else {
                        Mobile_Input_Callback::touches[id].state = 1;
					}
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
                    Mobile_Input_Callback::touches[id].state = state;
                    Mobile_Input_Callback::touches[id].x = x;
                    Mobile_Input_Callback::touches[id].y = y;
                    std::lock_guard<std::mutex> lock(touchMutex);
                    ThreadPool::AddMainThreadFunction([id](){
                        std::lock_guard<std::mutex> lock(touchMutex);
                        Mobile_Input_Callback::touches.erase(id);
                    });
                    break;
				}
				// Touch Cancel
				case 3: {
                    Mobile_Input_Callback::touches[id].state = state;
                                       Mobile_Input_Callback::touches[id].x = x;
                                       Mobile_Input_Callback::touches[id].y = y;
                    ThreadPool::AddMainThreadFunction([id](){
                        std::lock_guard<std::mutex> lock(touchMutex);
                        Mobile_Input_Callback::touches.erase(id);
                    });
					//Mobile_Input_Callback::touches[id].CallTouchEndFunctions(&Mobile_Input_Callback::touches[id]);
					
					break;
				}
            }
			mb->TouchEvent(id, state, x, y);

		}
#endif
	}
}



}
