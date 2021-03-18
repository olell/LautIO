/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#include <Arduino.h>

#include "util/log.h"
#include "util/http.h"

#include "driver/filesystem.h"
#include "driver/wifi.h"

void setup() {
    init_logging();
    init_filesystem();
    init_wifi();
    init_http();
}

void loop() {

}