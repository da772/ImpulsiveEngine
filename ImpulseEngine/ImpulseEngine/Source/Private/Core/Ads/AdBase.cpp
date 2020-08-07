#include "gepch.h"
#include "Public/Core/Ads/AdBase.h"
#include "Public/Core/Platform/Ads/Firebase/Firebase_Ads.h"

namespace GEngine {

	GEngine::Ad* Ad::Create(EAdPlatforms platform)
	{
		switch (platform) {
		
		case Firebase:
			return new Firebase_Ads();
		case None:
		default:
			GE_CORE_WARN("AD PLATFORM NOT SPECIFIED!");
			return nullptr;
		}

	}

}