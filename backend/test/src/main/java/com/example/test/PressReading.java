package com.example.test;
import com.example.test.service.DataService;
import org.eclipse.paho.client.mqttv3.MqttAsyncClient;
import org.eclipse.paho.client.mqttv3.*;
import org.springframework.stereotype.Component;

import java.util.UUID;

@Component
public class PressReading {

    public static MqttAsyncClient myClient;
    private final DataService service;

    public PressReading(DataService service)throws MqttException{
        this.service  = service;

        myClient = new MqttAsyncClient("tcp://192.168.43.121:1883",UUID.randomUUID().toString());
        PressCallback myCallback = new PressCallback(service);
        myClient.setCallback(myCallback);

        IMqttToken token = myClient.connect();
        token.waitForCompletion();

        myClient.subscribe("TOPIC/press", 0);
    }

    public static void pressReading() throws MqttException {

    }

}
