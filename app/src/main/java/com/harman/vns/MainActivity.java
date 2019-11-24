package com.harman.vns;

import androidx.appcompat.app.AppCompatActivity;

import android.view.View;
import android.widget.*;
import android.os.Bundle;
import android.widget.TextView;
import android.util.Log;
import androidx.annotation.Keep;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

public class MainActivity extends AppCompatActivity {

    int hour = 0;
    int minute = 0;
    int second = 0;
    TextView tickView;
    private Button mButton;



    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        tickView = findViewById(R.id.tickView);
        tickView.setText(stringFromJNI());

        mButton = findViewById(R.id.button);
        mButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
               // ping();
                //tickView.setText(stringFromJNI());
                StopTicks();
            }
        });
    }


    public  void ping()
    {

        try {

            Process shell = null;

            //   String[] cmdline = "ping -W 4 -c 1 www.google.com"; //"{"sh", "-c", "ping" ,"localhost" };

            shell = Runtime.getRuntime().exec("ping -W 20 -c 1 www.google.com");

            //shell = Runtime.getRuntime().exec("ls -la");


            String line = null;

            InputStream stderr = shell.getErrorStream();
            InputStreamReader esr = new InputStreamReader(stderr);
            BufferedReader ebr = new BufferedReader(esr);
            while ( (line = ebr.readLine()) != null )
                Log.e("FXN-BOOTCLASSPATH", line);

            InputStream stdout = shell.getInputStream();
            InputStreamReader osr = new InputStreamReader (stdout);
            BufferedReader obr = new BufferedReader (osr);
            while ( (line = obr.readLine()) != null )
                Log.i("FXN-BOOTCLASSPATH", line);

            int exitVal = shell.waitFor();
            Log.d("FXN-BOOTCLASSPATH", "getprop exitValue: " + exitVal);



        }
        catch (Exception e) {
            Log.d("PINGVALUE11", "ShellRoot#suExecute() finished with error", e);
        }

    }
    @Override
    public void onResume() {
        super.onResume();
        hour = minute = second = 0;
           ((TextView)findViewById(R.id.hellojniMsg)).setText(stringFromJNI());
        startTicks("www.yahoo.com");
    }

    @Override
    public void onPause () {
        super.onPause();
        StopTicks();
    }




    /*
     * A function calling from JNI to update current timer
     */
    @Keep
    private void updateTimer1(final String msg ) {

        Log.e("JniHandler1", "Native Err: " + msg);

        ++second;
        if(second >= 60) {
            ++minute;
            second -= 60;
            if(minute >= 60) {
                ++hour;
                minute -= 60;
            }
        }
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                String ticks = "" + MainActivity.this.hour + ":" +
                        MainActivity.this.minute + ":" +
                        MainActivity.this.second;
                MainActivity.this.tickView.setText(msg + " " +ticks);
            }
        });
    }


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */


    public native  String stringFromJNI();
    public native void startTicks(String inPar);
    public native void StopTicks();
}