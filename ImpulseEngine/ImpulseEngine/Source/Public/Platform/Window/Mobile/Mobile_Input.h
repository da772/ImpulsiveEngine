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

class Mobile_Input_Callback {
public:

	static inline void SetKeyboardTextFunc(const std::function<void(std::string)>& f) {
		Mobile_Input_Callback::setKeyboardTextFunc = f;
	}
    
    static inline void SetGetSafeArea(const std::function<void(int*,int*,int*,int*)>& f) {
        Mobile_Input_Callback::getSafeView = f;
    }

	static inline void SetGetKeyboardTextFunc(const std::function<std::string()>& f) {
		Mobile_Input_Callback::getKeyboardTextFunc = f;
	}

	static inline void SetShowKeyboardFunc(const std::function<void()>& f) {
		Mobile_Input_Callback::showKeyboardFunc = f;
	}

	static inline void SetHideKeyboardFunc(const std::function<void()>& f) {
		Mobile_Input_Callback::showKeyboardFunc = f;
	}
    
    static inline void SetGetViewContext(const std::function<ViewContext()>& f) {
        Mobile_Input_Callback::getViewContext = f;
    }
    
    static inline void BindView() {
        if (Mobile_Input_Callback::setBindView)
            Mobile_Input_Callback::setBindView();
    }
    
    static inline void SetSetBindView(const std::function<void()>& f) {
        Mobile_Input_Callback::setBindView = f;
    }

	static inline void ShowKeyboard() {
		if (showKeyboardFunc) {
			keyboardOpen = true;
			showKeyboardFunc();
		}
	}

	static inline void HideKeyboard() {
		if (hideKeyboardFunc) {
			keyboardOpen = false;
			hideKeyboardFunc();
		}
	}

	static inline const double GetTime() {
		if (Mobile_Input_Callback::getTimeFunc) {
			return Mobile_Input_Callback::getTimeFunc();
		}
		return 0;
	}

    static inline ViewContext GetViewContext() {
        if (Mobile_Input_Callback::getViewContext) {
            return Mobile_Input_Callback::getViewContext();
        }
    }
    
	static inline void SetKeyboardValue(const std::string& s) {
		if (Mobile_Input_Callback::setKeyboardTextFunc)
			Mobile_Input_Callback::setKeyboardTextFunc(s);
	}

	static inline std::string GetKeyboardValue() {
		if (Mobile_Input_Callback::getKeyboardTextFunc)
			return Mobile_Input_Callback::getKeyboardTextFunc();
		return "";
	}

	static inline void SetGetTimeFunc(const std::function<double()>& f) {
		Mobile_Input_Callback::getTimeFunc = f;
	}

	static inline const int16_t& GetXPos() {
		return xPos;
	}

	static inline const int16_t& GetYPos() {
		return yPos;
	}

	static inline bool GetTouchDown() {
		return touchDown;
	}

	static void Touched(uint64_t id, int state, float x, float y, float force);

	static inline void SetTouchEndFunction(uint64_t id, std::function<void(const FTouchInfo&)> f) {
		std::lock_guard<std::mutex> lock(Mobile_Input_Callback::touchMutex);
		std::unordered_map<uint64_t, FTouchInfo>::iterator it = Mobile_Input_Callback::touches.find(id);
		if (it != Mobile_Input_Callback::touches.end()) {
			it->second.AddTouchEndFunction(f);
		}
	}
	
    static inline std::unordered_map<uint64_t, FTouchInfo> GetTouches() {
		{
			std::lock_guard lock(Mobile_Input_Callback::touchMutex);
			std::unordered_map<uint64_t, FTouchInfo> t = Mobile_Input_Callback::touches;
			return t;
		}
    }
    
    static inline void ClearTouches() {
        std::lock_guard<std::mutex> lock(Mobile_Input_Callback::touchMutex);
        Mobile_Input_Callback::touches.clear();
    }
    
    static inline int GetTouchCount() {
		int size = 0;
		{
			std::lock_guard lock(Mobile_Input_Callback::touchMutex);
			size = (int)Mobile_Input_Callback::touches.size();
		}
		return size;
    }
    
    static inline void GetSafeArea(int* top, int* bottom, int* left, int* right) {
        if (getSafeView) {
            getSafeView(top, bottom, left, right);
        }
    }
    


private:
	static std::function<void()> showKeyboardFunc;
	static std::function<void()> hideKeyboardFunc;
	static std::function<void(std::string)> setKeyboardTextFunc;
    static std::function<ViewContext()> getViewContext;
    static std::function<void()> setBindView;
	static std::function<std::string()> getKeyboardTextFunc;
	static bool keyboardOpen, touchDown;
	static int16_t xPos, yPos;
	static std::function<double()> getTimeFunc;
    static std::mutex touchMutex;
    static std::unordered_map<uint64_t, FTouchInfo> touches;
    static std::function<void(int*,int*,int*,int*)> getSafeView;


};


	class GE_API Mobile_Input : public Input {
		public:
	static inline std::unordered_map<uint64_t, FTouchInfo> GetTouches() {
		return Mobile_Input_Callback::GetTouches();
    }
    
    static inline int GetTouchCount() {
		return Mobile_Input_Callback::GetTouchCount();
    }
        
        static inline void SetTouchEndFunction(uint64_t id, std::function<void(const FTouchInfo&)> f) {
            Mobile_Input_Callback::SetTouchEndFunction(id, f);
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
