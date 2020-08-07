#pragma once

#include "Public/Core/Core.h"

#ifdef GE_CONSOLE_APP
#include "Public/Core/Platform/Window/GLFW/MainEntryPoint.h"
#endif

#ifdef GE_MOBILE_APP
#include "Public/Core/Platform/Window/Mobile/MobileEntryPoint.h"
#endif
