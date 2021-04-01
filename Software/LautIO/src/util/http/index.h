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

void view_index(AsyncWebServerRequest *request) {
    /* This route returns the index html file */
    request->send(FSHANDLE, "/web/html/index.html", "text/html");
}

void view_about(AsyncWebServerRequest *request) {
    /* This route returns the about html file */
    request->send(FSHANDLE, "/web/html/about.html", "text/html");
}

void http_index_setup_routes(AsyncWebServer* server) {
    server->on("/", HTTP_GET, view_index);
    server->on("/about", HTTP_GET, view_about);
}