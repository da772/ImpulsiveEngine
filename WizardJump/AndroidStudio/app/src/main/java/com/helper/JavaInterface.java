package com.helper;

import android.app.Activity;
import android.os.Build;
import android.util.Log;
import android.view.DisplayCutout;
import android.view.View;
import android.view.ViewTreeObserver;
import android.view.WindowInsets;
import android.view.WindowManager;

import com.google.android.gms.ads.MobileAds;
import com.google.android.gms.ads.reward.RewardItem;
import com.google.android.gms.ads.reward.RewardedVideoAd;
import com.google.android.gms.ads.reward.RewardedVideoAdListener;

import java.lang.reflect.Method;

public class JavaInterface {

    public static final String CLASS_DISPLAY_CUTOUT = "android.view.DisplayCutout";
    public static final String METHOD_GET_DISPLAY_CUTOUT = "getDisplayCutout";
    public static final String FIELD_GET_SAFE_INSET_TOP = "getSafeInsetTop";
    public static final String FIELD_GET_SAFE_INSET_LEFT = "getSafeInsetLeft";
    public static final String FIELD_GET_SAFE_INSET_RIGHT = "getSafeInsetRight";
    public static final String FIELD_GET_SAFE_INSET_BOTTOM = "getSafeInsetBottom";

    static void SetRewardAdID(Activity a, String s) {
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

    static String GetSafeArea(Activity a) {

        try {
            WindowInsets windowInsets = null;
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                WindowManager.LayoutParams lp = a.getWindow().getAttributes();
                if (lp.layoutInDisplayCutoutMode != WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES) {
                    lp.layoutInDisplayCutoutMode = WindowManager.LayoutParams.LAYOUT_IN_DISPLAY_CUTOUT_MODE_SHORT_EDGES;
                }
                windowInsets = a.getWindow().getDecorView().getRootWindowInsets();
            }
            if (windowInsets == null) {
                return "NULL INSET";
            }
            Method method = WindowInsets.class.getMethod(METHOD_GET_DISPLAY_CUTOUT);
            Object displayCutout = method.invoke(windowInsets);
            if (displayCutout == null) {
                return "NULL CUTOUT";
            }
            Class clz = Class.forName(CLASS_DISPLAY_CUTOUT);
            int top = (int) clz.getMethod(FIELD_GET_SAFE_INSET_TOP).invoke(displayCutout);
            int left = (int) clz.getMethod(FIELD_GET_SAFE_INSET_LEFT).invoke(displayCutout);
            int right = (int) clz.getMethod(FIELD_GET_SAFE_INSET_RIGHT).invoke(displayCutout);
            int bottom = (int) clz.getMethod(FIELD_GET_SAFE_INSET_BOTTOM).invoke(displayCutout);
            return Integer.toString(top) + ":"+ Integer.toString(bottom) + ":"+Integer.toString(left) + ":" + Integer.toString(right) +":";
        } catch (Exception e) {
            Log.e("GetSafeArea", e.getMessage());
            return "0:0:0:0:";
        }

    }
}
