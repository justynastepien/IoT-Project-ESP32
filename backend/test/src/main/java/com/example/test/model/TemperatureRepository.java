package com.example.test.model;

import java.util.List;
import java.util.Optional;

public interface TemperatureRepository {

    Optional<Temperature> findById(Integer id);
    Temperature save(Temperature entity);
    List<Temperature> findAllByOrderByIdDesc();
}
