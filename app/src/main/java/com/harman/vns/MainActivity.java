package com.harman.vns;

import androidx.appcompat.app.AppCompatActivity;

import android.text.method.ScrollingMovementMethod;
import android.view.View;
import android.widget.*;
import android.os.Bundle;
import android.widget.TextView;
import android.util.Log;
import androidx.annotation.Keep;

public class MainActivity extends AppCompatActivity {

    int hour = 0;
    int minute = 0;
    int second = 0;
    TextView displayView;
    private Button mButtonStop;
    private Button mButtonStart;
    private EditText mHostNameId;


    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        displayView = findViewById(R.id.displayId);
        displayView.setMovementMethod(new ScrollingMovementMethod());
        mHostNameId = findViewById(R.id.hostNameId);
//        displayView.setText(stringFromJNI());

        mButtonStart = findViewById(R.id.buttonStart);
        mButtonStart.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mButtonStop.setEnabled(true);
                mButtonStart.setEnabled(false);
                displayView.setText("");
                String host = mHostNameId.getText().toString();
                if (host.isEmpty()) host = "www.google.com";


                LocationJNI(10.1,10.0);

                // ping();
                //displayView.setText(stringFromJNI());
                startTicks(host);
            }
        });

        mButtonStop = findViewById(R.id.buttonStop);
        mButtonStop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mButtonStart.setEnabled(true);
                mButtonStop.setEnabled(false);
                // ping();
                //displayView.setText(stringFromJNI());
                StopTicks();
            }
        });
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
    private void updateTimer(final String msg ) {

        Log.e("JniHandler1", "Native Err: " + msg);

        final String s = displayView.getText().toString();
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
//                String ticks = "" + MainActivity.this.hour + ":" +
//                        MainActivity.this.minute + ":" +
//                        MainActivity.this.second;
                MainActivity.this.displayView.append("\n" +msg);
//                MainActivity.this.displayView.setText(s + "\n" + msg);
            }
        });
    }


    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */


    public native  String stringFromJNI();
    public native void startTicks(String hostName);
    public native void StopTicks();
    public native void LocationJNI( double d1,  double d2);
}