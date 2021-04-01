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
#include "driver/wifi.h"

void view_scan_networks(AsyncWebServerRequest *request) {
    /*
    This route scans all networks visible for the ESP32 and returns
    a list of network names in JSON format.
    When there are no visible networks the status field becomes "error".
    */
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
    /* This route returns the wifi settings html file */
    request->send(FSHANDLE, "/web/html/settings/wifi.html", "text/html");
}

void view_wifi_set_credentials(AsyncWebServerRequest *request) {
    /* This route sets the wifi credentials and writes them to filesystem */
    String wifi_ssid = "";
    String wifi_pass = "";

    bool all_params = true;

    if (request->hasParam("ssid", true)) {
        AsyncWebParameter* ssid_param = request->getParam("ssid", true);
        wifi_ssid = ssid_param->value();
    } else all_params = false;

    if (request->hasParam("pass", true)) {
        AsyncWebParameter* pass_param = request->getParam("pass", true);
        wifi_pass = pass_param->value();
    } else all_params = false;


    if (!all_params) {
        request->send(200, "text/plain", "Missing parameter/s, please check your request!");
        return;
    }

    wifi_set_credentials(wifi_ssid.c_str(), wifi_pass.c_str());
    write_credentials_to_fs();

    log_debug("Configured wifi ssid: %s", wifi_ssid.c_str());
    log_debug("Configured wifi pass: %s", wifi_pass.c_str());

    request->redirect("/reboot"); // Todo reboot view
}

void http_wifi_setup_routes(AsyncWebServer* server) {
    server->on("/scan_networks", HTTP_GET, view_scan_networks);
    server->on("/settings/wifi", HTTP_GET, view_wifi_settings);
    server->on("/settings/wifi/set_credentials", HTTP_POST, view_wifi_set_credentials);
}