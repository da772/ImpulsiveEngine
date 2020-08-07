#pragma once
namespace GEngine {

	class Ad;

	class AdManager {

	public:
		static void Initialize();
		static void SetAdId(const char* id);
		static void SetRewardAdId(const char* id);
		static void LoadRewardAd(std::function<void()> f1, std::function<void(int, std::string)> f);
		static void ShowRewardAd();
		static void SetUserId(const char* id);

		static void Shutdown();

	private:
		static Ad* m_ad;


	};
}