package com.example.test.service;

import com.example.test.model.Pressure;
import com.example.test.model.PressureRepository;
import com.example.test.model.Temperature;
import com.example.test.model.TemperatureRepository;
import com.example.test.response.Response;
import org.springframework.aop.scope.ScopedProxyUtils;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;

@Service
public class DataService {
    private final TemperatureRepository temperatureRepository;
    private final PressureRepository pressureRepository;

    public DataService(TemperatureRepository repository, PressureRepository pressureRepository){
        this.temperatureRepository = repository;
        this.pressureRepository = pressureRepository;
    }

    public Response GettingData(){

        List<Temperature> temp = temperatureRepository.findAllByOrderByIdDesc();
        Temperature temperature = temp.get(0);

        List<Pressure> press = pressureRepository.findAllByOrderByIdDesc();

        Pressure pressure = press.get(0);
        System.out.println(pressure.getPressure());
        System.out.println(temperature.getTemperature());

        Response response = new Response(temperature.getTemperature(), pressure.getPressure());
        System.out.println(response.getPressure());

        return response;

    }

    public Temperature createTemperature(Temperature toCreate){
        Temperature result = temperatureRepository.save(toCreate);
        return result;
    }

    public Pressure createPressure(Pressure toCreate){
        Pressure result = pressureRepository.save(toCreate);
        return result;
    }
}
