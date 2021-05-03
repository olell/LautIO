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

#include "sys_config.h"

#define FS_SPIFFS 0
#define FS_SDCARD 1

#if FILESYSTEM_TYPE == FS_SPIFFS
    #include <SPIFFS.h>
    #define FSHANDLE SPIFFS
#elif FILESYSTEM_TYPE == FS_SDCARD
    #include <SD_MMC.h>
    #define FSHANDLE SD_MMC
#endif

void init_filesystem();