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

#include "driver/filesystem.h"

#include "util/log.h"
#include "util/config.h"

DynamicJsonDocument ui_config(512);
bool ui_config_loaded = false;

void load_ui_config() {

    const char* ui_config_name = Configuration::section("interface")["default"];

    log_info("Loading ui config: %s", ui_config_name);

    char* ui_config_path = (char*) malloc(11 + strlen(ui_config_name)); // 11 = length of "/interface/"
    sprintf(ui_config_path, "/interface/%s", ui_config_name);
    log_debug("ui config path: %s", ui_config_path);
    File config_file = FSHANDLE.open(ui_config_path);

    DeserializationError error = deserializeJson(ui_config, config_file);
    if (error) {
        log_error("Failed to read interface configuration.. please restart controller!");
        while(1) {}
    }

    log_debug("Successfully loaded interface configuration");

    config_file.close();

    ui_config_loaded = true;
}

const char* ui_get_quick_controls() {
    if (!ui_config_loaded) load_ui_config();
    DynamicJsonDocument response_json(512);
    response_json["controls"] = ui_config["quick_controls"];
    response_json["cmd"] = "ui_quick_controls";
    const uint8_t l = measureJson(response_json);
    char* response = (char*) malloc(l + 1);
    serializeJson(response_json, response, l + 1);
    return response;
}

const char* ui_interface_handler(DynamicJsonDocument input) {
    // handle interface commands with section ui
    log_debug("Handling UI command");
    const char* command = input["command"];
    if (strcmp(command, "get_quick_controls") == 0) {
        return ui_get_quick_controls();
    }
    else if (strcmp(command, "add_quick_control") == 0) {
        return "";
    }
    else if (strcmp(command, "rem_quick_control") == 0) {
        return "";
    }
    else {
        return "";
    }
}