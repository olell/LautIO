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

// utils
#include "util/log.h"
#include "util/config.h"
#include "util/ftp.h"
#include "util/http.h"
#include "util/websockets.h"

// drivers
#include "driver/amp.h"
#include "driver/filesystem.h"
#include "driver/wifi.h"
#include "driver/battery.h"
#include "driver/dsp.h"

void setup() {

    // System setup, starting with logging
    init_logging();
    log_info("Starting system setup!");
    init_amps(); // Starting amps first to prevent potential damages
    init_filesystem();
    Configuration::get_instance(); // Getting an instance to initialise
    init_wifi();
    init_battery();
    init_ftp_server();
    init_dsp();
    init_http(); // inits http & websockets
}

void loop() {
    // Try to do the most thing async.. but repeating stuff here
    ftp_server_loop();
    websocket_loop();
}