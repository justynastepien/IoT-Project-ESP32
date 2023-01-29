package com.example.test;

import com.example.test.model.Pressure;
import com.example.test.model.Temperature;
import com.example.test.service.DataService;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.springframework.stereotype.Component;

import javax.xml.crypto.Data;

public class PressCallback implements MqttCallback {

    private final DataService service;

    public PressCallback(DataService service){
        this.service = service;
    }

    public void connectionLost(Throwable arg0) {
        // TODO Auto-generated method stub

    }

    public void deliveryComplete(IMqttDeliveryToken arg0) {
        // TODO Auto-generated method stub

    }

    public void messageArrived(String topic, MqttMessage message) throws Exception {
        System.out.println(message);
        double press = Double.parseDouble(message.toString());
        Pressure pressure = new Pressure(press);
        service.createPressure(pressure);

    }
}
