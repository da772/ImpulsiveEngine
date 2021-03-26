#pragma once


namespace GEngine {

	enum class EAdPlatforms {
		None = 0,
		Firebase = 1
	};


	class GE_API Ad {

	public:
		virtual ~Ad() {};
		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;
		static Ad* Create(const EAdPlatforms platform);
		virtual void LoadRewardAd(const std::function<void()> f1 = nullptr) = 0;
		virtual bool AdLoaded() = 0;
		virtual void ShowRewardAd(std::function<void(int, std::string)> rewardCallback = nullptr, std::function<void(int)> endCallback = nullptr) = 0;
		virtual bool AdPlaying() = 0;
		inline virtual void SetUserId(const std::string id) { m_userId = id; }
		inline virtual void SetAdId(const std::string id) { m_adId = id; }
		inline virtual void SetRewardAdId(const std::string id) { m_rewardAdId = id; }
        inline virtual void SetTimeout(float f) {m_timeout = f; };
	protected:
		Ad() {};
        float m_timeout = 30.f;
		std::string m_adId = "";
		std::string m_rewardAdId = "";
		std::string m_userId = "";

	};


}
