#pragma once
#include "Public/Core/Ads/AdBase.h"
#ifdef GE_ADS_FIREBASE

#endif

namespace firebase {
	class App;
}

namespace GEngine {


	class Firebase_Ads : public Ad {
	public:
		Firebase_Ads();
		virtual ~Firebase_Ads();
		virtual void Initialize() override;
		virtual void Shutdown() override;
		virtual void LoadRewardAd(std::function<void()> f1, std::function<void(int, std::string)> f) override;
		virtual void ShowRewardAd() override;
		virtual void SetAdId(std::string s) override;
		virtual bool AdPlaying() override;

	private:
		firebase::App* m_app = nullptr;
		std::mutex m_initMutex;
		bool m_rewardAdLoaded = false;
		bool m_rewardAdInit = false;
		bool m_initalizing = false;
		


	};

}