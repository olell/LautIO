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

// drivers
#include "driver/amp.h"
#include "driver/filesystem.h"

void setup() {

    // System setup, starting with logging
    init_logging();
    log_info("Starting system setup!");
    init_amps(); // Starting amps first to prevent potential damages
    init_filesystem();
    Configuration::get_instance(); // Getting an instance to initialise

}

void loop() {
    // Try to do the most thing async.. but repeating stuff here
}