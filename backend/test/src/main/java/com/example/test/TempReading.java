package com.example.test;

import com.example.test.service.DataService;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttAsyncClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.springframework.stereotype.Component;

import java.util.UUID;

@Component
public class TempReading {

    public static MqttAsyncClient myClient;
    private final DataService service;

    public TempReading(DataService service)throws MqttException{
        this.service = service;

        myClient = new MqttAsyncClient("tcp://192.168.43.121:1883",UUID.randomUUID().toString());
        MyCallback myCallback = new MyCallback(this.service);
        myClient.setCallback(myCallback);

        IMqttToken token = myClient.connect();
        token.waitForCompletion();

        myClient.subscribe("TOPIC", 0);
        }

}
