/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#include "ESPAsyncWebServer.h"
#include "AsyncWebSocket.h"
#include "ArduinoJson.h"

#include "driver/filesystem.h"
#include "driver/dsp.h"

/* Routes */
void view_get_structure(AsyncWebServerRequest *request) {
    /* This route returns the dsp structure file as json */
    request->send(FSHANDLE, "/dsp/structure.json", "application/json");
}

void http_dsp_setup_routes(AsyncWebServer* server) {
    server->on("/dsp/get_structure", HTTP_GET, view_get_structure);
}

/* Websocket handling */
void websocket_dsp_handle_command(DynamicJsonDocument json_doc, AsyncWebSocketClient* client) {
    log_debug("Handling dsp websocket command");
    if (json_doc.containsKey("action")) {
        const char* action = json_doc["action"];

        // dsp actions
        if (strcmp(action, "update_volslew") == 0) {
            uint8_t id = 0;
            float volume = -50;
            uint8_t slew = 12;
            bool all_fields = true;

            if (json_doc.containsKey("id")) {
                id = json_doc["id"];
            } else all_fields = false;

            if (json_doc.containsKey("volume")) {
                volume = json_doc["volume"];
            } else all_fields = false;

            if (json_doc.containsKey("slew")) {
                slew = json_doc["slew"];
            } else all_fields = false;

            if (!all_fields) {
                client->text("{\"status\": \"error\", \"message\": \"missing field/s\"}");
                return;
            }

            dsp_update_volslew(id, volume, slew);

        }

    }
    else {
        client->text("{\"status\": \"error\", \"message\": \"missing action field\"}");
    }
}