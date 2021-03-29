#pragma once

#include "Public/Core/Core.h"

#if defined(GE_CONSOLE_APP) || defined (GE_SERVER_APP)
#include "Public/Platform/EntryPoint/MainEntryPoint.h"
#endif

#ifdef GE_MOBILE_APP
#include "Public/Platform/EntryPoint/MobileEntryPoint.h"
#endif
