package com.generalsx.zerohour;

import android.app.Activity;
import android.app.Application;
import android.os.Bundle;
import android.system.ErrnoException;
import android.system.Os;
import android.util.Log;

/**
 * Process-level safety net for the optional secondary mod overlay.
 *
 * GENERALSX_MOD_DIR is a process environment variable, so it can survive while
 * Android reuses this app process. Setup's Launch Game action is intentionally
 * a base-game launch; clear any previously selected mod whenever Setup is
 * entered or resumed so a mod can never leak into that path accidentally.
 */
public class GeneralsApplication extends Application {
    private static final String TAG = "GeneralsApplication";

    @Override
    public void onCreate() {
        super.onCreate();
        registerActivityLifecycleCallbacks(new ActivityLifecycleCallbacks() {
            @Override
            public void onActivityCreated(Activity activity, Bundle savedInstanceState) {
                clearModOverlayForSetup(activity, "created");
            }

            @Override
            public void onActivityResumed(Activity activity) {
                clearModOverlayForSetup(activity, "resumed");
            }

            @Override public void onActivityStarted(Activity activity) {}
            @Override public void onActivityPaused(Activity activity) {}
            @Override public void onActivityStopped(Activity activity) {}
            @Override public void onActivitySaveInstanceState(Activity activity, Bundle outState) {}
            @Override public void onActivityDestroyed(Activity activity) {}
        });
    }

    private static void clearModOverlayForSetup(Activity activity, String lifecycleState) {
        if (!(activity instanceof SetupActivity)) {
            return;
        }
        try {
            Os.unsetenv("GENERALSX_MOD_DIR");
            Log.i(TAG, "cleared stale mod overlay when Setup was " + lifecycleState);
        } catch (ErrnoException e) {
            Log.w(TAG, "failed to clear stale mod overlay when Setup was " + lifecycleState, e);
        }
    }
}
