#include "gepch.h"
#include "Public/Core/Ads/AdBase.h"
#include "Public/Core/Platform/Ads/Firebase/Firebase_Ads.h"

namespace GEngine {

	GEngine::Ad* Ad::Create(const EAdPlatforms platform)
	{
		switch (platform) {
		
		case EAdPlatforms::Firebase:
			return new Firebase_Ads();
		case EAdPlatforms::None:
		default:
			GE_CORE_WARN("AD PLATFORM NOT SPECIFIED!");
			return nullptr;
		}

	}

}