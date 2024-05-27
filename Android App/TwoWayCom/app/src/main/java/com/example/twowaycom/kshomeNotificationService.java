package com.example.twowaycom;

import android.app.Service;
import android.content.ComponentName;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;

import androidx.annotation.Nullable;

public class kshomeNotificationService extends Service {
    private static String TAG = "kshomeNotificationService";
    private Handler handler;
    private Runnable runnable;
    private final int runTime = 5000;

    @Override
    public void onCreate() {
        super.onCreate();
        Log.i(TAG,"OnCreate");
        handler = new Handler();
        runnable = new Runnable() {
            @Override
            public void run() {
                handler.postDelayed(runnable,runTime);
            }
        };
        handler.post(runnable);
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return START_STICKY;
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onDestroy() {
        if(handler!=null){
            handler.removeCallbacks(runnable);
        }
        super.onDestroy();

    }

}
