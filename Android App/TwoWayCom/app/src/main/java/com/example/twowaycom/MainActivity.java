package com.example.twowaycom;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.NotificationCompat;

import android.app.Application;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import android.widget.VideoView;

public class MainActivity extends AppCompatActivity {
    VideoView simpleVideoView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button btStart = findViewById(R.id.btnStart);
        if(btStart!= null){
            btStart.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Log.d("button", "Start Button clicked");
                    Toast.makeText(MainActivity.this," Start button clicked", Toast.LENGTH_LONG).show();
                    Intent intent1 = new Intent(MainActivity.this, kshomeNotificationService.class);
                    startService(intent1);
                }
            });
        }

        Button btStop = findViewById(R.id.btnStop);
        if(btStop!= null){
            btStop.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Log.d("button", " Stop Button clicked");
                    Toast.makeText(MainActivity.this,"Stop button clicked", Toast.LENGTH_LONG).show();
                    //Intent intent1 = new Intent(MainActivity.this, kshomeNotificationService.class);
                    //startService(intent1);


                }
            });
        }

        /*
        simpleVideoView  = (VideoView) findViewById(R.id.simpleVideoView);

        Uri uri = Uri.parse("https://www.youtube.com/watch?v=-WxDt1F1410");

        simpleVideoView.setVideoURI(uri);
        simpleVideoView.start();
         */
    }
    void pushNotification(){

        Intent activityIntent = new Intent(MainActivity.this,MainActivity.class);
        PendingIntent pendingIntent = PendingIntent.getActivity(MainActivity.this,1,activityIntent,PendingIntent.FLAG_IMMUTABLE);
        NotificationCompat.Builder notification = new NotificationCompat.Builder(MainActivity.this, kshomeNotificationChannel.CHANNEL_ID)
                .setSmallIcon(R.drawable.doorbell)
                .setContentTitle(kshomeNotificationChannel.textTitle)
                .setContentText(kshomeNotificationChannel.textContent)
                .setPriority(NotificationCompat.PRIORITY_DEFAULT)
                //.setSound()
                .setContentIntent(pendingIntent);
        NotificationManager notificationManager = (NotificationManager) getSystemService (Context.NOTIFICATION_SERVICE);
        notificationManager.notify("myNotification",1,notification.build());

    }
}