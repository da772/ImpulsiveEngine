#pragma once

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

	namespace Mobile_Interface {


		extern void GetSafeArea(int*, int*, int*, int*);
		extern void BindView();
		extern ViewContext GetViewContext();
		extern void ShowKeyboard();
		extern std::string GetKeyboardValue();
		extern void HideKeyboard();
		extern void SetKeyboardValue(const std::string&);
		extern float const GetTime();

	}


}