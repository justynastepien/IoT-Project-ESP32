package com.example.test.model;

import java.util.List;
import java.util.Optional;

public interface PressureRepository {

    Optional<Pressure> findById(Integer id);
    Pressure save(Pressure entity);
    List<Pressure> findAllByOrderByIdDesc();
}
