#include "gepch.h"
#include "Public/Core/Util/Time.h"

#include <chrono>

namespace GEngine {

	long long Time::GetEpochTimeNS()
	{
		return  std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();;
	}

	long long Time::GetEpochTimeMS()
	{
		return  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();;
	}

	double Time::GetEpochTimeSec()
	{
		return  std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() /1e9f;
	}

}
