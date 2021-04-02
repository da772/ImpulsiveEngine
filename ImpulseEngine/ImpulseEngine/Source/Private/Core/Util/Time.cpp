#include "gepch.h"
#include "Public/Core/Util/Time.h"

#include <chrono>

namespace GEngine {

	const uint64_t Time::GetEpochTimeNS()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}

	const uint64_t Time::GetEpochTimeMS()
	{
		return  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}

	const uint64_t Time::GetEpochTimeSec()
	{
		return  std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	}

}
