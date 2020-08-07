package com.helper;

import android.app.Activity;
import android.content.Context;
import android.util.Log;
import android.view.inputmethod.InputMethodManager;

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
                InputMethodManager imm = (InputMethodManager) a.getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.toggleSoftInput(InputMethodManager.HIDE_IMPLICIT_ONLY, 0);
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
