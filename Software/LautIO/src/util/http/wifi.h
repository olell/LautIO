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

#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#include "driver/filesystem.h"

void view_scan_networks(AsyncWebServerRequest *request) {
    String response_json = "{";

    int num = WiFi.scanNetworks();

    if (num == 0) {
        response_json += "\"status\":\"error\",";
        response_json += "\"message\":\"No networks found\"";
    }

    else {
        response_json += "\"status\":\"ok\",";
        response_json += "\"networks\":[";
        for (int i = 0; i < num; i++) {
            response_json += '"' + WiFi.SSID(i) + "\",";
        }
        response_json[response_json.length() - 1] = ']';
    }

    response_json += '}';
    request->send(200, "application/json", response_json);
}

void view_wifi_settings(AsyncWebServerRequest *request) {
    request->send(FSHANDLE, "/web/html/settings/wifi.html", "text/html");
}

void http_wifi_setup_routes(AsyncWebServer* server) {
    server->on("/scan_networks", HTTP_GET, view_scan_networks);
    server->on("/settings/wifi", HTTP_GET, view_wifi_settings);
}