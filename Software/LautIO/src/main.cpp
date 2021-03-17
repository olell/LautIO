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
#include "driver/filesystem.h"

void setup() {
    init_logging();
    init_filesystem();
}

void loop() {

}