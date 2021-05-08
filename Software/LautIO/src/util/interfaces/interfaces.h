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

const char* handle_interfaces(DynamicJsonDocument input) {
    const char* category = input["category"];
    if (strcmp(category, "dsp") == 0) {
        return dsp_interface_handler(input);
    }
    else {
        return "";
    }
}