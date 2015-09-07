//
//  Batt_management.cpp
//  Battery
//
//  Created by Hugz on 26/07/2015.
//  Copyright (c) 2015 Hugz. All rights reserved.
//

#include "Batt_management.h"

Batt_management::Batt_management(){
    
}

Batt_management::Batt_management(float capacity){
    
    batt_maxcapacity=capacity;
    min_capacity=0;
    Adafruit_INA219 ina219;
    charge_mAh=capacity;
    time_check = 0;
    time_low_volt=0;
    low_volt_duration=0;
    current_A=0;
    
}

void Batt_management::setup(){
    
    // Setup the INA219.
    ina219.begin();
    // By default the INA219 will be calibrated with a range of 32V, 2A.
    // However uncomment one of the below to change the range.  A smaller
    // range can't measure as large of values but will measure with slightly
    // better precision.
    //ina219.setCalibration_32V_1A();
    //ina219.setCalibration_16V_400mA();
}

void Batt_management::read(){
    uint32_t now_time = micros();
    
    // Read voltage and current from INA219.
    float shuntvoltage = ina219.getShuntVoltage_mV();
    float busvoltage = ina219.getBusVoltage_V();
    current_A = ina219.getCurrent_mA();
    
    // Compute load voltage, power, and milliamp-hours.
    voltage_V = busvoltage + (shuntvoltage / 1000.0);
    float power_mW = voltage_V * (current_A*1000.0);
    charge_mAh -= current_A*(time_check-now_time)/3600000000.0;
    
    time_check = micros(); //reset the new time of reading
}

float Batt_management::voltage() const {
    return  voltage_V;
}

float Batt_management::current() const {
    return  current_A;
}

float Batt_management::capacity_remaining() const {
    return charge_mAh;
}

float Batt_management::capacity_remaining_pct() const {
    return charge_mAh/batt_maxcapacity * 100.0 ;
}

bool Batt_management::is_exhausted(float low_voltage,float min_capacity){
    
    
    uint32_t now_time = micros();
    if ((voltage_V > 0) && (low_voltage>0) && voltage_V<low_voltage) // check if there is some voltage
    {
        
        if(time_low_volt == 0){ // first time below low_voltage so we set the timer
            time_low_volt = now_time;
        }
        else if ((now_time - time_low_volt) > low_volt_duration)
            return true;
    }else { //not exhausted so we reset the timer
        time_low_volt = 0;
    }
    
    if (current_A > 0 && charge_mAh < min_capacity){
        return true;
    }
    else {
        return false; // The battery is ok
    }
}

