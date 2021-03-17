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

#include <stdint.h>
#include "const.h"
#include "log.h"

// Network Configuration
#define WIFI_BEHAVIOR        WIFI_AP_CONFIG
#define WIFI_SSID            "" // Only required in WIFI_CONFIG_CRED mode
#define WIFI_PASSPHRASE      "" // Only required in WIFI_CONFIG_CRED mode

#define WIFI_AP_SSID         "LautIO"
#define WIFI_AP_PSK          "DeutschlandVerrecke"
#define WIFI_AP_IP           10,0,0,1 // Host IP in AP Mode

#define HOSTNAME             "lautio"

#define WIFI_CONNECT_TIMEOUT 30000 // 30 sec connect timeout

// Logging Configuration
#define LOG_SERIAL_BAUD      115200
#define LOG_LEVEL            LOGLEVEL_DEBUG

// Hardware configuration
#define HARDWARE_VERSION     HW_PROTO1


#if HARDWARE_VERSION == HW_PROTO1
    #include "hardware/hw_proto1.h"
#endif