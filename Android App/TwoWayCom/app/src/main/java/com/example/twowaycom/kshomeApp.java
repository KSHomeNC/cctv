package com.example.twowaycom;

import static com.example.twowaycom.kshomeNotificationChannel.CHANNEL_ID;
import static com.example.twowaycom.kshomeNotificationChannel.textContent;
import static com.example.twowaycom.kshomeNotificationChannel.textTitle;

import android.app.Application;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.Handler;
import android.util.Log;
import android.widget.Toast;

import androidx.core.app.NotificationCompat;

public class kshomeApp extends Application {
    public static kshomeNotificationChannel.NOTIFY_TYPE notifyType;
    @Override
    public void onCreate() {
        super.onCreate();
        createChannel();
        notifyType = kshomeNotificationChannel.NOTIFY_TYPE.NOTIFY_TYPE_BELL;
    }
    void createChannel(){
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {

            int importance = NotificationManager.IMPORTANCE_DEFAULT;
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID, kshomeNotificationChannel.CHANNEL_NAME, importance);
            channel.setDescription(kshomeNotificationChannel.description);
            // Register the channel with the system; you can't change the importance
            // or other notification behaviors after this.
            NotificationManager notificationManager = getSystemService(NotificationManager.class);
            //NotificationManager notificationManager = ( NotificationManager) getSystemService(MainActivity.NOTIFICATION_SERVICE);
            notificationManager.createNotificationChannel(channel);
            notificationHandler();
            Log.d("Noti Channel","Notification Channel created");
            Toast.makeText(this,"channel created", Toast.LENGTH_LONG).show();
        }
    }

    public void postNotification(kshomeNotificationChannel.NOTIFY_TYPE type){
        notifyType = type;
    }

    private void notificationHandler() {
        Runnable runnable = new Runnable() {
            @Override
            public void run() {

                synchronized (this) {
                    try {
                        wait(10000);
                        //Toast.makeText(getApplicationContext(), "notification about to send", Toast.LENGTH_LONG).show();
                        if (notifyType != kshomeNotificationChannel.NOTIFY_TYPE.NOTIFY_TYPE_NONE) {
                            switch (notifyType) {
                                case NOTIFY_TYPE_BELL:
                                    Log.d("app", " Notification sending");
                                    sendBellNotification();
                                    Log.d("app", " Notification sent");
                                    break;
                                case NOTIFY_TYPE_MOTION:
                                    sendMotionNotification();
                                    break;
                                default:
                                    break;
                            }
                            notifyType = kshomeNotificationChannel.NOTIFY_TYPE.NOTIFY_TYPE_NONE;
                        }

                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }

            }
        };
        Thread thread = new Thread(runnable);
        thread.start();
    }

    private void sendMotionNotification() {
    }

    private void sendBellNotification() {
        Context context = getApplicationContext();
        Intent activityIntent = new Intent( context  ,MainActivity.class);
        PendingIntent pendingIntent = PendingIntent.getActivity(context,1,activityIntent,PendingIntent.FLAG_IMMUTABLE);
        NotificationCompat.Builder notification = new NotificationCompat.Builder(context, CHANNEL_ID)
                .setSmallIcon(R.drawable.doorbell)
                .setContentTitle(textTitle)
                .setContentText(textContent)
                .setPriority(NotificationCompat.PRIORITY_DEFAULT)
                //.setSound()
                .setContentIntent(pendingIntent);
        NotificationManager notificationManager = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
        notificationManager.notify("myNotification",1,notification.build());
        Log.d("app"," Notification meg build for sent");

    }

}
