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

// driver
#include "driver/filesystem.h"

// util
#include "util/http.h"
#include "util/websockets.h"
#include "util/log.h"


AsyncWebServer webserver(HTTP_PORT);

void serve_web_ui(AsyncWebServerRequest *request) {
    const char* file_name = request->pathArg(0).c_str();
    char* sd_path = (char*) malloc(5 + strlen(file_name)); // len("/web/") == 5
    sprintf(sd_path, "/web/%s", file_name);

    if(FSHANDLE.exists(sd_path)){
        AsyncWebServerResponse *response = request->beginResponse(FSHANDLE, sd_path);
        response->addHeader("Cache-Control", "max-age=31536000");
        request->send(response);
    } else request->send(404);
}


void init_http() {
    log_info("Initialising HTTP"); 

    // Handler
    init_websocket(&webserver);

    // Routes
    webserver.on("^\\/(.*)$", HTTP_GET, serve_web_ui);
    webserver.rewrite("/", "/index.html");

    // Start webserver
    webserver.begin();
    log_info("Started HTTP Server");

}