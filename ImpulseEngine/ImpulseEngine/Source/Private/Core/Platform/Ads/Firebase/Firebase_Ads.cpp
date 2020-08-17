#include "gepch.h"
#include "Public/Core/Platform/Ads/Firebase/Firebase_Ads.h"

#ifdef GE_ADS_FIREBASE
#include "firebase/admob.h"
#include "firebase/admob/types.h"
#include "firebase/app.h"
#include "firebase/future.h"
#include "firebase/admob/rewarded_video.h"
#endif

#ifdef GE_PLATFORM_ANDROID
#include "Public/Core/Platform/Window/Android/android_util.h"
#include "Public/Core/Platform/Window/Mobile/Mobile_Input.h"
#endif

#ifdef GE_PLATFORM_IOS
#include "Public/Core/Platform/Window/Mobile/Mobile_Input.h"
#endif


#include "Public/Core/Util/ThreadPool.h"
#include "Public/Core/Util/Time.h"


#ifdef GE_ADS_FIREBASE
class LoggingRewardedVideoListener
	: public firebase::admob::rewarded_video::Listener {
public:
	LoggingRewardedVideoListener()  {};
	virtual ~LoggingRewardedVideoListener() {};
	void OnRewarded(firebase::admob::rewarded_video::RewardItem reward) override {
		if (m_callback) {
			m_callback(reward.amount, reward.reward_type);
		}
		GE_CORE_DEBUG("REWARDING USER WITH: {0},{1} ", reward.amount, reward.reward_type.c_str());
		//firebase::admob::rewarded_video::Destroy();
	}
	void OnPresentationStateChanged(
		firebase::admob::rewarded_video::PresentationState state) override {
		GE_CORE_DEBUG("REWARDED VIDEO HAS CHANGED TO: {0} ", state);
	}
	void SetCallback(std::function<void(int, std::string)> f) {
		m_callback = f;
	}

private:
	std::function<void(int, std::string)> m_callback;
};
LoggingRewardedVideoListener* rewarded_video_listener = new LoggingRewardedVideoListener();
firebase::admob::AdRequest my_ad_request;
#endif

namespace GEngine {

	Firebase_Ads::Firebase_Ads()
	{

	}

	Firebase_Ads::~Firebase_Ads()
	{
#ifdef GE_PLATFORM_ANDROID
		if (m_rewardAdInit) {
			AndroidUtil::CleanJNIEnv();
		}
#endif
	}

	void Firebase_Ads::Initialize()
	{
#ifdef GE_ADS_FIREBASE
		if (m_adId != "") {
			if (m_app) {
				GE_CORE_ASSERT(!m_app, "FIREBASE APP ALREADY CREATED!");
				return;
			}
#ifdef GE_PLATFORM_ANDROID
			m_app =
				firebase::App::Create(firebase::AppOptions(),
					AndroidUtil::GetJNIEnv(),
					Mobile_Input_Callback::GetViewContext());
#endif
#ifdef GE_PLATFORM_IOS
			m_app = firebase::App::Create(firebase::AppOptions());
#endif
			ThreadPool::AddJob([this]() {
				{
					std::lock_guard<std::mutex> guard(m_initMutex);
					m_initalizing = true;
				}
				
				firebase::admob::Initialize(*m_app, m_adId.c_str());
				double ct = Time::GetEpochTimeSec();
				{
					std::lock_guard<std::mutex> guard(m_initMutex);
					if (!m_rewardAdInit) {
						firebase::admob::rewarded_video::Initialize();

						while (firebase::admob::rewarded_video::InitializeLastResult().status() == 1) {
							if (Time::GetEpochTimeSec() - ct > 30.0) {
								GE_CORE_ERROR("Firebase: Failed to initialize reward video");
								return;
							}
						}
						m_rewardAdInit = true;
						GE_CORE_DEBUG("Reward Video Initalized!");
					}
					if (m_userId.size() > 0) {
						ThreadPool::AddMainThreadFunction([this]() {
#ifdef GE_PLATFORM_ANDROID
							AndroidUtil::SetRewardUserId_Google(m_userId);
#endif
							std::lock_guard<std::mutex> guard(m_initMutex);
							m_initalizing = false;

							});
					}
					
				}

				GE_CORE_INFO("Firebase App Initialized!");
				});
				}
		else {
			GE_CORE_WARN("Firebase Ad ID is not set!");
			return;
		}
		
		
#endif
	}

	void Firebase_Ads::Shutdown()
	{
#ifdef GE_ADS_FIREBASE
		if (m_app) {
			firebase::admob::rewarded_video::Destroy();
			delete m_app;
		}
#endif
	}

	void Firebase_Ads::LoadRewardAd(std::function<void()> f1, std::function<void(int, std::string)> f)
	{
#ifdef GE_ADS_FIREBASE
		ThreadPool::AddJob([this, f,f1]() {
			double ct = Time::GetEpochTimeSec();
			
			bool bInit = false;
			{
				std::lock_guard<std::mutex> guard(m_initMutex);
				bInit = m_initalizing;
			}
			{
				
				if (bInit) {
					while (bInit) {
						{
							if (Time::GetEpochTimeSec() - ct > 30.0) {
								break;
							}
							std::lock_guard<std::mutex> guard(m_initMutex);
							bInit = m_initalizing;
						}
					}
					if (bInit) {
						GE_CORE_ERROR("FAILED TO INITALIZE REWARD AD");
						return;
					}
					
				}
			}

			GE_CORE_INFO("Firebase App Reward Video Initialized!");
			rewarded_video_listener->SetCallback(f);
			firebase::admob::rewarded_video::SetListener(rewarded_video_listener);
			
			const char* devices[] = { "TEST_EMULATOR" };
			my_ad_request.test_device_ids = devices;
			my_ad_request.test_device_id_count = 1;
			firebase::admob::rewarded_video::LoadAd(m_rewardAdId.c_str(), my_ad_request);
			firebase::FutureStatus status = firebase::admob::rewarded_video::LoadAdLastResult().status();
			ct = Time::GetEpochTimeSec();
			while ((status = firebase::admob::rewarded_video::LoadAdLastResult().status()) == firebase::kFutureStatusPending) {
				if (Time::GetEpochTimeSec() - ct > 30.0) {
					GE_CORE_ERROR("Firebase: Failed to initialize reward video");
					return;
				}
			};

			if (status == firebase::kFutureStatusInvalid) {
				GE_CORE_ERROR("Firebase: Failed to load Reward Video!");
				return;
			}

			{
				std::lock_guard<std::mutex> guard(m_initMutex); 
				m_rewardAdLoaded = true;
			}
			if (f1) {
				ThreadPool::AddMainThreadFunction(f1);
			}
			GE_CORE_INFO("Firebase: Reward Video Loaded!");
		});
#endif
	}

	void Firebase_Ads::ShowRewardAd()
	{
#ifdef GE_ADS_FIREBASE
		ThreadPool::AddJob([this]() {
			std::lock_guard<std::mutex> guard(m_initMutex);
			if (!m_rewardAdInit) {
				GE_CORE_ERROR("Firebase: Reward Ad module not initalized");
				return;
			}

			if (!m_rewardAdInit) {
				GE_CORE_ERROR("Firebase: Reward Ad not loaded calls LoadRewardAd()");
				return;
			}

			firebase::admob::rewarded_video::Show(Mobile_Input_Callback::GetViewContext());

			});
        #endif
	}

	void Firebase_Ads::SetAdId(std::string s)
	{
		Ad::SetAdId(s);
		if (m_app)
			Shutdown();
		Initialize();
	}

}
