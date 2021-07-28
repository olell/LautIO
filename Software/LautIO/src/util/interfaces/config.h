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

#include "util/config.h"
#include "util/log.h"

const char* _config_get_section(DynamicJsonDocument input) {
    const char* section = input["section"];
    DynamicJsonDocument response(1024);
    response["content"] = Configuration::section(section);
    // blurring passwords.. todo maybe some smarter way
    if (strcmp(section, "wifi") == 0) {
        response["content"]["pass"] = "********";
        response["content"]["ap_pass"] = "********";
    }
    else if (strcmp(section, "ftp") == 0) {
        response["content"]["password"] = "********";
    }
    response["cmd"] = "config_get_section";
    response["section"] = section;
    size_t json_size = measureJson(response) + 1;
    char* response_text = (char*) malloc(json_size);
    serializeJson(response, response_text, json_size);
    return response_text;
}

const char* _config_update_field(DynamicJsonDocument input) {
    const char* section = input["section"];
    const char* field = input["field"];
    JsonVariant val = input["value"];

    Configuration::section(section)[field] = val;
    Configuration::write();
    return "{\"status\": \"ok\", \"message\": \"done!\", \"cmd\": \"config_update_field\"}";
}

const char* config_interface_handler(DynamicJsonDocument input) {
    // handle interface commands with section config
    log_debug("Handling config command");
    const char* command = input["command"];
    if (strcmp(command, "get_section") == 0) {
        return _config_get_section(input); // returns selected section content
    }
    else if (strcmp(command, "update_field") == 0) {
        return _config_update_field(input);
    }
    else {
        return "";
    }
}