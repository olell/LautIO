/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#include "config.h"
#include "driver/filesystem.h"

#if FILESYSTEM_TYPE == FS_SPIFFS
#include <SPIFFS.h>
void init_filesystem() {
    log_info("Initialising SPIFFS filesystem");
    if(!SPIFFS.begin(true)) {
        log_fatal("An fatal error occured while mounting SPIFFS. Please reset controller!");
        while(true) {}; // Halt
    }
    log_debug("SPIFFS up and running!");
}
#endif

#if FILESYSTEM_TYPE == FS_SDCARD
void init_filesystem() {
    // TODO SD card stuffs
}
#endif