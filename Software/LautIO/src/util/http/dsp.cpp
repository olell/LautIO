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
#include "SigmaDSP.h"

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

        else if (strcmp(action, "update_soeq") == 0) {
            uint8_t id = 0;
            float boost = 0;
            float gain = 0;
            uint16_t freq = 440;
            float Q = 1.41;
            float S = 0;
            float bandwidth = 0;
            uint8_t state = 0;
            uint8_t phase = 0;
            uint8_t filter_type = 0;

            bool all_fields = true;

            if (json_doc.containsKey("id")) id = json_doc["id"];
            else all_fields = false;

            if (json_doc.containsKey("boost")) boost = json_doc["boost"];
            else all_fields = false;

            if (json_doc.containsKey("gain")) gain = json_doc["gain"];
            else all_fields = false;

            if (json_doc.containsKey("freq")) freq = json_doc["freq"];
            else all_fields = false;

            if (json_doc.containsKey("Q")) Q = json_doc["Q"];
            else all_fields = false;

            if (json_doc.containsKey("S")) S = json_doc["S"];
            else all_fields = false;

            if (json_doc.containsKey("bandwidth")) bandwidth = json_doc["bandwidth"];
            else all_fields = false;

            if (json_doc.containsKey("state")) state = json_doc["state"];
            else all_fields = false;

            if (json_doc.containsKey("phase")) phase = json_doc["phase"];
            else all_fields = false;

            if (json_doc.containsKey("filter_type")) filter_type = json_doc["filter_type"];
            else all_fields = false;

            if (!all_fields) {
                client->text("{\"status\": \"error\", \"message\": \"missing field/s\"}");
                return;
            }

            secondOrderEQ eq;

            eq.Q = Q;
            eq.S = S;
            eq.bandwidth = bandwidth;
            eq.gain = gain;
            eq.freq = freq;
            eq.boost = boost;
            if (state == 0) eq.state = parameters::state::off;
            else if (state == 1) eq.state = parameters::state::on;

            if (phase == 0) eq.phase = parameters::phase::nonInverted;
            else if (phase == 1) eq.phase = parameters::phase::inverted;

            if (filter_type == 0) eq.filterType = parameters::filterType::peaking;
            else if (filter_type == 1) eq.filterType = parameters::filterType::lowpass;
            else if (filter_type == 2) eq.filterType = parameters::filterType::highpass;
            else if (filter_type == 3) eq.filterType = parameters::filterType::bandpass;
            else if (filter_type == 4) eq.filterType = parameters::filterType::bandstop;

            dsp_update_soeq(id, eq);

        }

    }
    else {
        client->text("{\"status\": \"error\", \"message\": \"missing action field\"}");
    }
}