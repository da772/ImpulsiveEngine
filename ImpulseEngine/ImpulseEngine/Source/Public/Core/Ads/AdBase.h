#pragma once


namespace GEngine {

	enum class EAdPlatforms {
		None = 0,
		Firebase = 1
	};


	class Ad {

	public:
		virtual ~Ad() {};
		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;
		static Ad* Create(EAdPlatforms platform);
		virtual void LoadRewardAd(std::function<void()> f1, std::function<void(int, std::string)> f) = 0;
		virtual void ShowRewardAd() = 0;

		inline virtual void SetUserId(std::string id) { m_userId = id; }
		inline virtual void SetAdId(std::string id) { m_adId = id; }
		inline virtual void SetRewardAdId(std::string id) { m_rewardAdId = id; }


	protected:
		Ad() {};
		
		std::string m_adId = "";
		std::string m_rewardAdId = "";
		std::string m_userId = "";

	};


}
