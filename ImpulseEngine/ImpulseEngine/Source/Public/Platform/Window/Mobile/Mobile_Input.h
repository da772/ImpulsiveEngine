#pragma once
#ifndef DEF_MOBILE_INPUT
#define DEF_MOBILE_INPUT
#include "Public/Core/Application/Input.h"
#ifdef GE_PLATFORM_IOS
#include <objc/objc.h>
typedef id ViewContext;
#elif GE_PLATFORM_ANDROID
#include "Public/Platform/Window/Android/android_native_app_glue.h"
typedef jobject ViewContext;
#else
typedef int ViewContext;
#endif


namespace GEngine {

	struct FTouchInfo {
	public:
		FTouchInfo() {};
		FTouchInfo(uint64_t id, int state, float x, float y, float force, long long time) :
			id(id), state(state), x(x), y(y), force(force), time(time) {

		}

		uint64_t id;
		int state;
		long long time;
		float x, y, force;

		inline void AddTouchEndFunction(std::function<void(const FTouchInfo&)> f) { touchEndFunctions.push_back(f); }
		inline void CallTouchEndFunctions(FTouchInfo* _f) {
			for (const std::function<void(const FTouchInfo&)>& f : touchEndFunctions) {
				if (f)
					f(*_f);
                else
                    GE_CORE_ASSERT(f, "INVALID FUNCTION");
			}
			touchEndFunctions.clear();
		};
		inline uint64_t GetID() { return id; }
		inline float GetX() { return x; }
		inline float GetY() { return y; }
		inline float GetForce() { return force; }
		inline int GetState() { return state; }
		inline long long GetTime() { return time; }

private:
	std::vector<std::function<void(const FTouchInfo&)>> touchEndFunctions;

};

class Mobile_Touch_Callback {
public:

	static inline bool GetTouchDown() {
		return touchDown;
	}

	static void Touched(uint64_t id, int state, float x, float y, float force);
	static void ProcessKeyboard(uint32_t action, uint32_t keycode);

	static inline void SetTouchEndFunction(uint64_t id, std::function<void(const FTouchInfo&)> f) {
		std::lock_guard<std::mutex> lock(Mobile_Touch_Callback::touchMutex);
		std::unordered_map<uint64_t, FTouchInfo>::iterator it = Mobile_Touch_Callback::touches.find(id);
		if (it != Mobile_Touch_Callback::touches.end()) {
			it->second.AddTouchEndFunction(f);
		}
	}
	
    static inline std::unordered_map<uint64_t, FTouchInfo> GetTouches() {
		{
			std::lock_guard lock(Mobile_Touch_Callback::touchMutex);
			std::unordered_map<uint64_t, FTouchInfo> t = Mobile_Touch_Callback::touches;
			return t;
		}
    }
    
    static inline void ClearTouches() {
        std::lock_guard<std::mutex> lock(Mobile_Touch_Callback::touchMutex);
        Mobile_Touch_Callback::touches.clear();
    }
    
    static inline int GetTouchCount() {
		int size = 0;
		{
			std::lock_guard lock(Mobile_Touch_Callback::touchMutex);
			size = (int)Mobile_Touch_Callback::touches.size();
		}
		return size;
    }
    
    


private:
	static bool keyboardOpen, touchDown;
	static std::mutex touchMutex;
	static std::unordered_map<uint64_t, FTouchInfo> touches;


};


	class GE_API Mobile_Input : public Input {
		public:
	static inline std::unordered_map<uint64_t, FTouchInfo> GetTouches() {
		return Mobile_Touch_Callback::GetTouches();
    }
    
    static inline int GetTouchCount() {
		return Mobile_Touch_Callback::GetTouchCount();
    }
        
	static inline void SetTouchEndFunction(uint64_t id, std::function<void(const FTouchInfo&)> f) {
		Mobile_Touch_Callback::SetTouchEndFunction(id, f);
	}
        
    static void ClearTouches();

	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(uint64_t button) override;
		virtual float GetMouseXImpl(uint64_t id) override;
		virtual float GetMouseYImpl(uint64_t id) override;
		virtual std::pair<float, float> GetMousePositionImpl(uint64_t id) override;
		virtual void ProcessEvent(const Event& e) override;

	};



}



#endif
