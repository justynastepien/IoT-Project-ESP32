package com.example.test.controller;


import com.example.test.response.Response;
import com.example.test.service.DataService;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.net.URI;

@RestController
@CrossOrigin(origins = "http://localhost:3000", maxAge = 3600)
public class DataController {
    private final DataService service;

    public DataController(DataService service){
        this.service = service;
    }

    @GetMapping("/data")
    public ResponseEntity<?> readData(){
        Response response = service.GettingData();
        if(response != null){
            return ResponseEntity.ok(response);
        }

        return ResponseEntity.notFound().build();
    }

//    @PostMapping("/data")
//    ResponseEntity<Data> createData(@RequestBody Data toCreate){
//        Data result = service.createData(toCreate);
//        return ResponseEntity.created(URI.create("/" + result.getId())).body(result);
//    }
}
