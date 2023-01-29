package com.example.test.model;

import lombok.*;

import javax.persistence.*;

@Entity
@Table(name="pressure")
@NoArgsConstructor
//@AllArgsConstructor
public class Pressure {

    public Pressure(double pressure){
        this.pressure = pressure;
    }

    @Id
    @Getter(value = AccessLevel.PUBLIC)
    @Setter(value = AccessLevel.PUBLIC)
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;
    @Getter(value = AccessLevel.PUBLIC)
    @Setter(value = AccessLevel.PUBLIC)
    private double pressure;
}
