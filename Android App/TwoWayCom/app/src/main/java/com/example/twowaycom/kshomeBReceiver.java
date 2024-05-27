package com.example.twowaycom;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.widget.Toast;

public class kshomeBReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        if(Intent.ACTION_BOOT_COMPLETED.equals(intent.getAction()) ){
            Intent activityIntent = new Intent(context, MainActivity.class);
            activityIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            context.startActivity(activityIntent);
        }
        else if(Intent.ACTION_AIRPLANE_MODE_CHANGED.equals(intent.getAction())){
            Toast.makeText(context.getApplicationContext(), "AirPlane mode change",Toast.LENGTH_LONG).show();

        }
        else{
            Toast.makeText(context.getApplicationContext(), "unhandled broad cast received",Toast.LENGTH_LONG).show();
        }

    }
}
