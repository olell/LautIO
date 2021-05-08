/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#include <ESPAsyncWebServer.h>

// http
#include "http.h"
#include "websockets.h"

// util
#include "util/log.h"


AsyncWebServer webserver(HTTP_PORT);


void init_http() {
    log_info("Initialising HTTP");

    // Routes

    // Handler
    init_websocket(&webserver);

    // Start webserver
    webserver.begin();
    log_info("Started HTTP Server");

}