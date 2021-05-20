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
#include "util/interfaces/dsp.h"
#include "util/interfaces/amp.h"
#include "util/interfaces/config.h"
#include "util/interfaces/system.h"

const char* handle_interfaces(DynamicJsonDocument input) {
    const char* category = input["category"];
    if (strcmp(category, "dsp") == 0) {
        return dsp_interface_handler(input);
    }
    else if (strcmp(category, "amp") == 0) {
        return amp_interface_handler(input);
    }
    else if (strcmp(category, "config") == 0) {
        return config_interface_handler(input);
    }
    else if (strcmp(category, "system") == 0) {
        return system_interface_handler(input);
    }
    else {
        return "";
    }
}