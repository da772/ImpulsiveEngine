#include "gepch.h"
#include "Public/Platform/Window/Mobile/Mobile_Interface.h"


namespace GEngine {
    namespace Mobile_Interface {
#ifndef GE_MOBILE_APP

        void GetSafeArea(int*, int*, int*, int*) {};
		void BindView() {};
		ViewContext GetViewContext() { return 0;};
		void ShowKeyboard() {};
		std::string GetKeyboardValue() { return "";};
		void HideKeyboard() {};
		void SetKeyboardValue(const std::string&) {};
		float const GetTime() { return 0.f;};
    }
#endif

}