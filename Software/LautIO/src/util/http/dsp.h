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

#include "ESPAsyncWebServer.h"

#include "driver/filesystem.h"

void view_get_structure(AsyncWebServerRequest *request) {
    /* This route returns the dsp structure file as json */
    request->send(FSHANDLE, "/dsp/structure.json", "application/json");
}

void http_dsp_setup_routes(AsyncWebServer* server) {
    server->on("/dsp/get_structure", HTTP_GET, view_get_structure);
}