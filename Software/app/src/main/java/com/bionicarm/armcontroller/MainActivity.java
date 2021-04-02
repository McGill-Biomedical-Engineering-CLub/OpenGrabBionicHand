package com.bionicarm.armcontroller;

import androidx.appcompat.app.AppCompatActivity;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;


public class MainActivity extends AppCompatActivity {
    static final UUID mUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    int outputKey = 0;
    BluetoothAdapter bt;
    BluetoothDevice hc = null;
    BluetoothSocket btSocket = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button Button0 = (Button) findViewById(R.id.button0);
        Button Button1 = (Button) findViewById(R.id.button1);
        Button Button2 = (Button) findViewById(R.id.button2);
        Button Button3 = (Button) findViewById(R.id.button3);
        ConnectToBluetooth();
        SendToBluetooth(0);
        Button0.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                SendToBluetooth((1));
            }
        });
        Button1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                SendToBluetooth((2));
            }
        });
        Button2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                SendToBluetooth((2));
            }
        });
        Button3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                SendToBluetooth((3));
            }
        });
    }
    private void ConnectToBluetooth(){
        bt = BluetoothAdapter.getDefaultAdapter();
        System.out.println(bt.getBondedDevices());
        hc = bt.getRemoteDevice("00:13:EF:10:1D:68");
        System.out.println(hc.getName());
        int counter = 0;
        do {
            try {
                btSocket = hc.createRfcommSocketToServiceRecord(mUUID);
                System.out.println(btSocket);
                btSocket.connect();
                ;
                System.out.println(btSocket.isConnected());
            } catch (IOException e) {
                e.printStackTrace();
            }
            counter++;
        } while((!btSocket.isConnected())&& counter<3);
    }
    private void SendToBluetooth(int key){
        try {
            OutputStream outputStream = btSocket.getOutputStream();
            outputStream.write(48+ key);
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}