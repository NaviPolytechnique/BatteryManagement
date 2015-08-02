//
//  Batt_management.h
//  Battery
//
//  Created by Hugz on 26/07/2015.
//  Copyright (c) 2015 Hugz. All rights reserved.
//

#ifndef __Battery__Batt_management__
#define __Battery__Batt_management__

#include <stdio.h>
#include <chrono>
#include <Wire.h>
#include <AP_HAL.h>
#include <AP_Common.h>
#include <AP_Math.h>
#include <Adafruit_INA219.h>

class Batt_management {
    
protected :
    int pin_volt;           //the pin number to read the voltage
    int pin_current;        //the pin to read the current
    
    float batt_maxcapacity; //the maximum capacity
    float min_capacity;     //minimum capacity accepted
    
    
    
public:
    
    // \brief constructor
    
    Batt_management();
    
    Batt_management(int pinV, int pinC, float capacity);
    
    struct Batt_state{ //Battery state
        
        float voltage_V;    //voltage in Volt
        float current_A;    //current in Ampere
        float charge_mAh;   //current charge of the battery in milli Ampere hours
        uint32_t time_check;       //last time the battery was checked
        uint32_t time_low_volt;    //time of low voltage
        
        uint32_t low_volt_duration;
        
    };
    
    // read voltage and current and compute charge
    void read();
    
    // return current voltage
    float voltage();
    
    
    float current();
    
    float capacity_remaining();
    
    float capacity_remaining_pct();
    
    bool is_exhausted(float low_voltage,float min_capacity);
};
#endif /* defined(__Battery__Batt_management__) */
