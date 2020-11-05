#include "gepch.h"

#include "Public/Core/Hardware/DeviceInfo.h"



namespace GEngine {
	


	const uint16_t DeviceInfo::GetCpuCount()
	{
		return (uint16_t)thread::hardware_concurrency();
	}


	const float DeviceInfo::GetCpuFreq()
	{
		return 0.0f;
	}

}