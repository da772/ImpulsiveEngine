#pragma once
namespace GEngine {

	class Ad;

	class AdManager {

	public:
		static void Initialize();
		static void SetAdId(const char* id);
		static void SetRewardAdId(const char* id);
		static void LoadRewardAd(const std::function<void()> onLoadCallback = nullptr);
		static void ShowRewardAd(std::function<void(int, std::string)> rewardCallback = nullptr, std::function<void(int)> endCallback = nullptr);
		static void SetUserId(const char* id);
		static bool AdPlaying();
		static void Shutdown();
		static bool AdLoaded();

	private:
		static Ad* m_ad;


	};
}