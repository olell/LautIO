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
#include "util/log.h"
#include "sys_config.h"

const char* system_interface_handler(DynamicJsonDocument input) {
    // handle interface commands with section system
    log_debug("Handling system command");
    const char* command = input["command"];
    if (strcmp(command, "restart") == 0) {
        ESP.restart();
        return "{\"status\": \"ok\", \"message\": \"done!\", \"cmd\": \"system_restart\"}"; // not actually required
    }
    if (strcmp(command, "log") == 0) {
        const char* msg = input["msg"];
        const char* level = input["level"];
        if (strcmp(level, "debug") == 0)
            log_debug("EXT: %s", msg);
        if (strcmp(level, "info") == 0)
            log_info("EXT: %s", msg);
        if (strcmp(level, "warning") == 0)
            log_warn("EXT: %s", msg);
        if (strcmp(level, "error") == 0)
            log_error("EXT: %s", msg);
        if (strcmp(level, "fatal") == 0)
            log_fatal("EXT: %s", msg);
        return "{\"status\": \"ok\", \"message\": \"done!\", \"cmd\": \"system_log\"}";
    }
    if (strcmp(command, "led") == 0) {
        uint8_t state = input["state"];
        digitalWrite(SYS_LED_PIN, state);
        return "{\"status\": \"ok\", \"message\": \"done!\", \"cmd\": \"system_led\"}";
    }
    else {
        return "";
    }
}