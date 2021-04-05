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
#include "util/websockets.h"

#include "driver/filesystem.h"
#include "driver/wifi.h"
#include "driver/dsp.h"

void setup() {
    init_logging();
    init_filesystem();
    init_wifi();
    init_http();
    init_dsp();
}

void loop() {
    websocket_loop();
    delay(10); // reduce cpu usage
}