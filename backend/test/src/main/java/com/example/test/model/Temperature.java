package com.example.test.model;

import lombok.*;

import javax.persistence.*;

@Entity
@Table(name="temperature")
@NoArgsConstructor
@AllArgsConstructor
public class Temperature {

    public Temperature(double temperature){
        this.temperature = temperature;
    }

    @Id
    @Getter(value = AccessLevel.PUBLIC)
    @Setter(value = AccessLevel.PUBLIC)
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;
    @Getter(value = AccessLevel.PUBLIC)
    @Setter(value = AccessLevel.PUBLIC)
    private double temperature;


}
