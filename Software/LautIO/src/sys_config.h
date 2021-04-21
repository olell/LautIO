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

// LOG config
#define LOG_LEVEL LOGLEVEL_DEBUG // todo: this should be configurable while runtime. Will change it later.

// Hardare configuration
#define HARDWARE_VERSION HW_PROTO1

#if HARDWARE_VERSION == HW_PROTO1
    #include "hardware_config/hw_proto1.h"
#endif

#ifdef HARDWARE_HAS_SDCARD
    #define FILESYSTEM_TYPE FS_SDCARD
#else
    #define FILESYSTEM_TYPE FS_SPIFFS
#endif