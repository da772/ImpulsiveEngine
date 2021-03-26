#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <fstream>
#include <sstream>
#include <array>
#include <map>
#include <set>
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <queue>
#include <list>
#include <regex>
#include <tuple>



#include "Public/Core/Application/Log.h"

#ifdef GE_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#endif

#ifdef GE_PLATFORM_MACOSX
	#include <libproc.h>
#endif

#ifdef GE_PROFILE
//#include "Public/Core/Profiling/Profiler.h"
#endif
