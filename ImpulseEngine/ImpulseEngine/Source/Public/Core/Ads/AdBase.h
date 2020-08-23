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
		static Ad* Create(const EAdPlatforms platform);
		virtual void LoadRewardAd(const std::function<void()> f1, const std::function<void(int, std::string)> f) = 0;
		virtual void ShowRewardAd() = 0;

		inline virtual void SetUserId(const std::string id) { m_userId = id; }
		inline virtual void SetAdId(const std::string id) { m_adId = id; }
		inline virtual void SetRewardAdId(const std::string id) { m_rewardAdId = id; }


	protected:
		Ad() {};
		
		std::string m_adId = "";
		std::string m_rewardAdId = "";
		std::string m_userId = "";

	};


}
