package com.example.twowaycom;



import android.app.Activity;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Build;

import androidx.core.app.NotificationChannelCompat;
import androidx.core.app.NotificationCompat;

public class kshomeNotificationChannel {
        static final String CHANNEL_ID = "KSHOME_CH";
        static final String CHANNEL_NAME = "KSHOME";
        static final String description = " Doorbell notification";
        static final String textTitle = " Door Bell";
        static final String textContent = " Door Bell ringing ";
        Context context;

        public enum NOTIFY_TYPE{
            NOTIFY_TYPE_BELL,
            NOTIFY_TYPE_MOTION,
            NOTIFY_TYPE_NONE
        }


} // main class

