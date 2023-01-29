package com.example.test;

import com.example.test.service.DataService;
import org.eclipse.paho.client.mqttv3.*;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.eclipse.paho.client.mqttv3.MqttAsyncClient;


import javax.xml.crypto.Data;
import java.nio.charset.StandardCharsets;
import java.util.UUID;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

@SpringBootApplication
public class TestApplication {

	public static MqttAsyncClient myClient;
	private final DataService service;

	public TestApplication(DataService service){
		this.service = service;
	}

	public static void main(String[] args){
		SpringApplication.run(TestApplication.class, args);
//		PressReading press = new PressReading();
		//PressReading.pressReading();



	}
}








