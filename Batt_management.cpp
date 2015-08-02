//
//  Batt_management.cpp
//  Battery
//
//  Created by Hugz on 26/07/2015.
//  Copyright (c) 2015 Hugz. All rights reserved.
//

#include "Batt_management.h"
#include <AP_HAL.h>
#include <AP_Common.h>
#include <AP_Math.h>

Batt_management(){
    
}

Batt_management::Batt_management(int pinV, int pinC, float capacity){
    pin_volt = pinV;
    pin_current= pinC;
    batt_maxcapacity=capacity;
    
}

void Batt_management::setup{

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
    uint32_t now_time = hal.scheduler->micros();
    
    // Read voltage and current from INA219.
    float shuntvoltage = ina219.getShuntVoltage_mV();
    float busvoltage = ina219.getBusVoltage_V();
    current_A = ina219.getCurrent_mA() * 1000;
    
    // Compute load voltage, power, and milliamp-hours.
    voltage_V = busvoltage + (shuntvoltage / 1000);
    float power_mW = loadvoltage * current_mA;
    charge_mAh -= current_mA*(time_check-now_time)/3600000000;
    
    time_check = hal.scheduler->micros(); //reser the new time of reading
}

float Batt_management::voltage() const {
    return const Batt_state.voltage_V;
}

float Batt_management::current() const {
    return const Batt_state.currant_A;
}

float Batt_management::capacity_remaining() const {
    return Batt_state.charge_mAh;
}

float Batt_management::capacity_remaining_pct() const {
    return Batt_state.charge_mAh/batt_maxcapacity;
}

bool Batt_management::is_exhausted(float low_voltage,float min_capacity){
    
    
    uint32_t now_time = hal.scheduler->micros();
    if ((Batt_state.voltage_V > 0) && (low_voltage>0) && Batt_state.voltage_V<low_voltage) // check if there is some voltage
    {
        
        if(Batt_state.time_low_volt == 0){ // first time below low_voltage so we set the timer
            Batt_state.time_low_volt = now_time;
        }
        else if ((now_time - Batt_state.time_low_volt) > Batt_state.low_volt_duration)
            return true;
    }else { //not exhausted so we reset the timer
        Batt_state.time_low_volt = 0;
    }

    if (Batt_state.current_A > 0 && Batt_state.charge_mAh < min_capacity){
        return true;
    }
    else {
        return false; // The battery is ok
    }
}



