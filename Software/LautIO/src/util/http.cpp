/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#include <WiFi.h>
#include <AsyncTCP.h>
#include <driver/filesystem.h>

#include <ESPAsyncWebServer.h>

#include "util/http.h"
#include "config.h"
#include "const.h"

#include "util/http/wifi.h"

AsyncWebServer server(HTTP_PORT);

void init_http() {
    log_info("Starting http init");

    // Todo: Static file serving

    http_wifi_setup_routes(&server);

    server.begin();
    log_debug("HTTP up and running");
}