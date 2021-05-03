/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#include "WiFi.h"

#include "sys_config.h"

// util
#include "util/log.h"
#include "util/config.h"
#include "driver/wifi.h"

bool wifi_connect_to_network() {
    /*
    This method tries to connect to the configured wifi network, it returns the success status as boolean
    */

    bool success = false;

    const char* ssid = Configuration::section("wifi")["ssid"];
    const char* pass = Configuration::section("wifi")["pass"];

    log_info("Starting to connect to WiFi network: %s", ssid);
    WiFi.begin(ssid, pass);

    unsigned long connection_start_time = millis();
    unsigned long t_now = 0;

    wl_status_t wifi_status = WiFi.status();
    while (wifi_status != WL_CONNECTED) {

        if (wifi_status == WL_DISCONNECTED) {
            // Do nothing, waiting for connection
            t_now = millis();
            if (t_now - connection_start_time > 30000) { // 30 seconds timeout.. todo maybe this should be part of the config :O
                // Connection timeout reached
                log_warn("Couldn't connect.. connection timeout reached!");
                success = false;
                break;
            }
        }

        else if (wifi_status == WL_CONNECTED) {
            log_info("WiFi connection successfully established");
            log_info("My IP addr is: %s", WiFi.localIP().toString().c_str());

            success = true;
            break;
        }

        else if (wifi_status == WL_NO_SSID_AVAIL) {
            log_warn("SSID \"%s\" not found..", ssid);
            success = false;
            break;
        }

        else if (wifi_status == WL_CONNECT_FAILED) {
            // todo: issue https://github.com/olell/LautIO/issues/7 happens here

            log_warn("WiFi connection failed, maybe invalid credentials?");
            success = false;
            break;
        }

        delay(500);
        wifi_status = WiFi.status();

    }

    return success;

}

bool wifi_start_accesspoint() {
    /*
    This method starts an accesspoint with the configured credentials
    */
}

void init_wifi() {

    // Load all base config parameters
    const char* mode = Configuration::section("wifi")["mode"];

    if (mode == WIFI_CONFIG_PARAM_CLIENT) {

        // Load client mode specific config parameter
        const bool fallback_ap = Configuration::section("wifi")["fallback_ap"];
        
        bool success = wifi_connect_to_network();

        if (!success && fallback_ap) {
            // start fallback access point
            wifi_start_accesspoint();
        }

    }

    else if (mode == WIFI_CONFIG_PARAM_ACCESSPOINT) {

    }

}