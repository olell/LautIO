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

#include "ArduinoJson.h"

#include "driver/amp.h"
#include "util/log.h"

const char* _amp_get_status() {
    DynamicJsonDocument amp_status(256);
    amp_status["AB"]["reset_state"] = get_amp_reset_state(AMP_AB);
    amp_status["AB"]["otw_state"] = get_amp_otw_state(AMP_AB);
    amp_status["AB"]["fault_state"] = get_amp_fault_state(AMP_AB);

    amp_status["CD"]["reset_state"] = get_amp_reset_state(AMP_CD);
    amp_status["CD"]["otw_state"] = get_amp_otw_state(AMP_CD);
    amp_status["CD"]["fault_state"] = get_amp_fault_state(AMP_CD);

    amp_status["cmd"] = "amp_status";
    char* response = (char*) malloc(256);
    serializeJson(amp_status, response, 256);
    return response;
}

const char* _amp_set_reset_state(DynamicJsonDocument input) {
    const char* amp_name = input["amp"];
    uint8_t reset_state = input["state"];
    if (strcmp(amp_name, "AB") == 0)
        amp_set_reset_state(AMP_AB, reset_state);
    if (strcmp(amp_name, "CD") == 0)
        amp_set_reset_state(AMP_CD, reset_state);
    return "{\"status\": \"ok\", \"message\": \"done!\", \"cmd\": \"amp_set_reset_state\"}";
}

const char* amp_interface_handler(DynamicJsonDocument input) {
    // handle interface commands with section amp
    log_debug("Handling amp command");
    const char* command = input["command"];
    if (strcmp(command, "get_status") == 0) {
        return _amp_get_status();
    }
    else if (strcmp(command, "set_reset_state") == 0) {
        return _amp_set_reset_state(input);
    }
    else {
        return "";
    }
}