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

const char* handle_interfaces(DynamicJsonDocument input) {
    const char* category = input["category"];
    if (strcmp(category, "dsp") == 0) {
        return dsp_interface_handler(input);
    }
    if (strcmp(category, "amp") == 0) {
        return amp_interface_handler(input);
    }
    else {
        return "";
    }
}