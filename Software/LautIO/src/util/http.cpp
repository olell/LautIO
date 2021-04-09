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

/* Handler */
#include "util/websockets.h"

/* Routes */
#include "util/http/wifi.h"
#include "util/http/index.h"
#include "util/http/dsp.h"

AsyncWebServer server(HTTP_PORT);

void serve_static(AsyncWebServerRequest *request) {
    // todo: this is very hacky, you can do it better olel!
    String dir_name = request->pathArg(0);
    String file_name = request->pathArg(1);

    if (dir_name.equals("..") || dir_name.equals("/") || file_name.equals("..") || file_name.equals("/")) {
        request->send(200, "text/plain", "Nahh, don't do this you silly, bad boy!");
    }

    String path = "/web/" + dir_name + "/" + file_name;
    String mime;
    
    if (dir_name == "css") mime = "text/css";
    if (dir_name == "js") mime = "application/javascript";
    if (dir_name == "svg") mime = "image/svg+xml";
    if (dir_name == "png") mime = "image/png";
    if (dir_name == "tmpl") mime = "text/plain";

    log_debug("Handling request for static file %s", path.c_str());
    request->send(SPIFFS, path, mime);
}

void init_http() {
    log_info("Starting http init");

    // Static file serving
    server.on("^\\/static\\/(.*)\\/(.*)$", HTTP_GET, serve_static);

    // Routes
    http_wifi_setup_routes(&server);
    http_index_setup_routes(&server);
    http_dsp_setup_routes(&server);

    // Handler
    init_websockets(&server);

    server.begin();
    log_debug("HTTP up and running");
}