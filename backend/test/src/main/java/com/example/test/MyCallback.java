package com.example.test;

import com.example.test.model.Pressure;
import com.example.test.model.Temperature;
import com.example.test.service.DataService;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class MyCallback implements MqttCallback {

        private final DataService service;

        public MyCallback(DataService service){
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
                double temp = Double.parseDouble(message.toString());
                Temperature temperature = new Temperature(temp);
                service.createTemperature(temperature);

        }
}
