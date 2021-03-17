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

#include "config.h"

#define FS_SPIFFS 0
#define FS_SDCARD 1

#if FILESYSTEM_TYPE == FS_SPIFFS
    #define FSHANDLE SPIFFS
#elif FILESYSTEM_TYPE == FS_SDCARD
    #define FSHANDLE SDCARD // TODO
#endif

void init_filesystem();