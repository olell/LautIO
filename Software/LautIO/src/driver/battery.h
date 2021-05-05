/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#pragma once

#include "Arduino.h"
#include "sys_config.h"

void init_battery() {
    //
    pinMode(VBAT_MEASURE, INPUT);
}

float get_battery_voltage() {
    // Get battery voltage in volts
    uint16_t adc_input = analogRead(VBAT_MEASURE);
    return adc_input / 77.28301886792453;
}