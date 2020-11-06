#include "gepch.h"
#include "Public/Core/Ads/AdManager.h"
#include "Public/Core/Ads/AdBase.h"




namespace GEngine {

	Ad* AdManager::m_ad = nullptr;

	void AdManager::Initialize()
	{
		if (m_ad == nullptr) {
			m_ad = Ad::Create(static_cast<EAdPlatforms>(GE_AD_PLATFORM));
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

    void AdManager::SetTimeout(float f) {
        if (m_ad)
        {
            m_ad->SetTimeout(f);
        }
    }

	void AdManager::SetRewardAdId(const char* id)
	{
		if (m_ad)
		{
			m_ad->SetRewardAdId(id);
		}
	}

	void AdManager::LoadRewardAd(std::function<void()> f1)
	{
		if (AdManager::m_ad != nullptr) {
			AdManager::m_ad->LoadRewardAd(f1);
		}
	}

	void AdManager::ShowRewardAd(std::function<void(int, std::string)> rewardCallback, std::function<void(int)> endCallback)
	{
		if (AdManager::m_ad != nullptr) {
			m_ad->ShowRewardAd(rewardCallback,endCallback);
		}
	}

	void AdManager::SetUserId(const char* id)
	{
		if (m_ad) {
			m_ad->SetUserId(id);
		}
	}

	bool AdManager::AdPlaying()
	{
		if (m_ad) {
			return m_ad->AdPlaying();
		}
		return false;
	}

	void AdManager::Shutdown()
	{
		if (m_ad != nullptr) {
			m_ad->Shutdown();
			delete m_ad;
		}
	}

	bool AdManager::AdLoaded()
	{
		if (m_ad) {
			return m_ad->AdLoaded();
		}
		return false;
	}

}
