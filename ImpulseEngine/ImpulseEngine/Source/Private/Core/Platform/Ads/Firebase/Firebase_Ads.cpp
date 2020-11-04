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
#include "Public/Core/Platform/Window/Mobile/MobileEntryPoint.h"


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
	}
	void OnPresentationStateChanged(firebase::admob::rewarded_video::PresentationState state) override {
		GE_CORE_DEBUG("REWARDED VIDEO HAS CHANGED TO: {0} ", state);
		if (state == 1) {
			App_Stop();
		}
		else if (state == 0) {
			App_Start();
			if (m_stateCallback) {
				m_stateCallback((int)state);
			}
				
		}
	}
	void SetRewardCallback(std::function<void(int, std::string)> f) {
		m_callback = f;
	}
	void SetStateChangeCallback(std::function<void(int)> f) {
		m_stateCallback = f;
	}

private:
	std::function<void(int, std::string)> m_callback;
	std::function<void(int)> m_stateCallback;
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
#if defined (GE_PLATFORM_ANDROID) && defined(GE_ADS_FIREBASE)
		if (m_rewardAdInit) {
			AndroidUtil::CleanJNIEnv();
			delete rewarded_video_listener;
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
				auto ct = Time::GetEpochTimeSec();
				{
					std::lock_guard<std::mutex> guard(m_initMutex);
					if (!m_rewardAdInit) {
						firebase::admob::rewarded_video::Initialize();

						while (firebase::admob::rewarded_video::InitializeLastResult().status() == 1) {
							if (Time::GetEpochTimeSec() - ct > m_timeout) {
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

	void Firebase_Ads::LoadRewardAd(std::function<void()> f1)
	{
#ifdef GE_ADS_FIREBASE
		ThreadPool::AddJob([this, f1]() {
			auto ct = Time::GetEpochTimeSec();
			
			bool bInit = false;
			{
				std::lock_guard<std::mutex> guard(m_initMutex);
				bInit = m_initalizing;
			}
			{
				
				if (bInit) {
					while (bInit) {
						{
							if (Time::GetEpochTimeSec() - ct > m_timeout) {
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
			{
				std::lock_guard<std::mutex> guard(m_initMutex);
				if (m_rewardAdLoaded)
					return;
			}
            {
                ct = Time::GetEpochTimeSec();
                bool b = false;
                while(!b) {
                    if (Time::GetEpochTimeSec() - ct > m_timeout) {
                        GE_CORE_ERROR("Firebase: Failed to initialize reward video");
                        return;
                    }
                    std::lock_guard<std::mutex> guard(m_initMutex);
                    b = m_rewardAdInit;
                }
                
            }
			GE_CORE_INFO("Firebase App Reward Video Initialized!");
			firebase::admob::rewarded_video::SetListener(rewarded_video_listener);
			
			const char* devices[] = { "TEST_EMULATOR" };
			my_ad_request.test_device_ids = devices;
			my_ad_request.test_device_id_count = 1;
			firebase::Future<void> fut = firebase::admob::rewarded_video::LoadAd(m_rewardAdId.c_str(), my_ad_request);
			firebase::FutureStatus status = fut.status();
			ct = Time::GetEpochTimeSec();

			if (fut.error() != 0) {
				GE_CORE_ERROR("Firebase: failed to load video ad. ERR: {0}", fut.error_message());
				return;
			}

			while ((status = firebase::admob::rewarded_video::LoadAdLastResult().status()) == firebase::kFutureStatusPending) {
				if (Time::GetEpochTimeSec() - ct > m_timeout) {
					GE_CORE_ERROR("Firebase: Failed to load reward video");
					return;
				}
			};

			if (status == firebase::kFutureStatusInvalid) {
				GE_CORE_ERROR("Firebase: Failed to load Reward Video!");
				return;
			}

			if (status != firebase::kFutureStatusComplete) {
				GE_CORE_ERROR("Firebase: Failed to load reward video");
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

	void Firebase_Ads::ShowRewardAd(std::function<void(int, std::string)> rewardCallback, std::function<void(int)> endCallback)
	{
#ifdef GE_ADS_FIREBASE
		ThreadPool::AddJob([this, rewardCallback, endCallback]() {
			std::lock_guard<std::mutex> guard(m_initMutex);
			if (!m_rewardAdInit) {
				GE_CORE_ERROR("Firebase: Reward Ad module not initalized");
				return;
			}

			if (!m_rewardAdLoaded) {
				GE_CORE_ERROR("Firebase: Reward Ad not loaded call LoadRewardAd()");
				return;
			}

			rewarded_video_listener->SetRewardCallback(rewardCallback);
			rewarded_video_listener->SetStateChangeCallback(endCallback);
			firebase::Future<void> f = firebase::admob::rewarded_video::Show(Mobile_Input_Callback::GetViewContext());
			
			if (f.error() != 0) {
				GE_CORE_ERROR("Error: {0}", f.error_message());
				return;

			}
			auto ct = Time::GetEpochTimeMS();
			while (Time::GetEpochTimeMS() - ct < 500) {
				continue;
			}

			{
				m_rewardAdLoaded = false;
			}

			if (firebase::admob::rewarded_video::presentation_state() == 0) {
				GE_CORE_ERROR("Firebase: Failed to show reward ad! {0}", f.error_message());
			}
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

	bool Firebase_Ads::AdPlaying()
	{
#ifdef GE_ADS_FIREBASE
		std::lock_guard<std::mutex> guard(m_initMutex);
		if (m_app && m_rewardAdInit) {
			return firebase::admob::rewarded_video::presentation_state() != 0;
		}
#endif
		return false;
	}

	bool Firebase_Ads::AdLoaded()
	{
#ifdef GE_ADS_FIREBASE
		std::lock_guard<std::mutex> guard(m_initMutex);
		return m_rewardAdLoaded;
#endif
		return false;
	}

}
