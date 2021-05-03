/*
 *
 * This file is part of LautIO
 * 
 * Only add parameters to this config file which
 * may not be configured while runtime! #DEFINEs are prefered!
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#pragma once

#include "util/log.h"

// Configuration config
#define CONFIG_FILE_PATH = "/config/config.json"

// LOG config (not included in config files because needed before any files are loaded)
#define LOG_SERIAL_BAUD 115200
#define LOG_LEVEL LOGLEVEL_DEBUG // todo: this should be configurable while runtime. Will change it later.

// Hardare configuration
#include "hardware_config/hw_proto2.h"

#ifdef HARDWARE_HAS_SDCARD
    #define FILESYSTEM_TYPE FS_SDCARD
#else
    #define FILESYSTEM_TYPE FS_SPIFFS
#endif