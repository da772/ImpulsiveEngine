package com.helper;

import android.app.Activity;
import android.util.Log;

import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.reward.RewardItem;
import com.google.android.gms.ads.reward.RewardedVideoAd;
import com.google.android.gms.ads.reward.RewardedVideoAdListener;

public class GoogleAd {

    static void SetID(Activity a, String s) {
        Log.d("Setting ID SET", s);
        a.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                RewardedVideoAd mAd = MobileAds.getRewardedVideoAdInstance(a);
                if (mAd != null) {
                    mAd.setUserId(s);
                } else {
                    Log.e("RewardedVideoAd", "Cannot Set UserID");
                }
            }
        });


    }

}
