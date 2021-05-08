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

#include "driver/dsp.h"
#include "util/log.h"

const char* _dsp_get_controls() {
    DynamicJsonDocument controls = *get_controls_json();
    controls["status"] = "ok";
    controls["cmd"] = "dsp_controls";
    char* response = (char*) malloc(1024);
    serializeJson(controls, response, 1024);
    return response;
}

const char* _dsp_update_control(DynamicJsonDocument input) {
    dsp_update_from_updated_json(input.as<JsonObject>());
    return "{\"status\": \"ok\", \"message\": \"done!\"}";
}

const char* dsp_interface_handler(DynamicJsonDocument input) {
    // handle interface commands with section dsp
    log_debug("Handling DSP command");
    const char* command = input["command"];
    if (strcmp(command, "get_controls") == 0) {
        return _dsp_get_controls();
    }
    if (strcmp(command, "update_control") == 0) {
        return _dsp_update_control(input);
    }
    else {
        return "";
    }
}