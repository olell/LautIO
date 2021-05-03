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

// drivers
#include "driver/amp.h"

void setup() {

  init_logging();
  log_info("Starting system setup!");
  init_amps(); // Starting amps first to prevent potential damages

}

void loop() {
}