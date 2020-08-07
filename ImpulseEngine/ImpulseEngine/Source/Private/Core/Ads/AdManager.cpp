#include "gepch.h"
#include "Public/Core/Ads/AdManager.h"
#include "Public/Core/Ads/AdBase.h"




namespace GEngine {

	Ad* AdManager::m_ad = nullptr;

	void AdManager::Initialize()
	{
		if (m_ad == nullptr) {
			m_ad = Ad::Create(static_cast<EAdPlatforms>(GE_AD_PLATFORM));
            if (m_ad)
                m_ad->Initialize();
			return;
		}
		GE_CORE_ASSERT(false, "ADS ALREADY CREATED");
	}

	void AdManager::SetAdId(const char* id)
	{
		if (m_ad)
		{
			m_ad->SetAdId(id);
		}
	}

	void AdManager::SetRewardAdId(const char* id)
	{
		if (m_ad)
		{
			m_ad->SetRewardAdId(id);
		}
	}

	void AdManager::LoadRewardAd(std::function<void()> f1, std::function<void(int, std::string)> f)
	{
		if (AdManager::m_ad != nullptr) {
			AdManager::m_ad->LoadRewardAd(f1,f);
		}
	}

	void AdManager::ShowRewardAd()
	{
		if (AdManager::m_ad != nullptr) {
			m_ad->ShowRewardAd();
		}
	}

	void AdManager::SetUserId(const char* id)
	{
		if (m_ad) {
			m_ad->SetUserId(id);
		}
	}

	void AdManager::Shutdown()
	{
		if (m_ad != nullptr) {
			m_ad->Shutdown();
			delete m_ad;
		}
	}

}
