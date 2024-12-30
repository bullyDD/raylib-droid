package com.example.a3dapp;

import androidx.appcompat.app.AppCompatActivity;

import android.app.NativeActivity;
import android.os.Bundle;
import android.widget.TextView;

import com.example.a3dapp.databinding.ActivityMainBinding;

public class MainActivity extends NativeActivity {

    // Used to load the 'main' library on application startup.
    static {
        System.loadLibrary("main");
    }

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

    }
}