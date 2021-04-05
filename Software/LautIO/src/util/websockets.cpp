/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#include <Arduino.h>
#include <AsyncWebSocket.h>

#include "util/log.h"
#include "driver/dsp.h"
#include "ArduinoJson.h"

#include "util/http/dsp.h"

AsyncWebSocket ws("/ws");
unsigned long ws_last_cleanup_millis = 0;

void handle_websocket_data(const char* message, AsyncWebSocketClient* client) {
    /* This method handles all incoming websocket data (on websocket 0.0.0.0/ws) */
    DynamicJsonDocument json_document(1024);
    deserializeJson(json_document, message);
    if (json_document.containsKey("command")) { // contains a command
        const char* command = json_document["command"];
        log_debug("Received command: %s", command);

        if (strcmp(command, "dsp") == 0) { // dsp command
            websocket_dsp_handle_command(json_document, client);
        }
        else if (strcmp(command, "test") == 0) { // test command
            log_debug("Handling test websocket command");
            client->text("{\"status\": \"ok\", \"message\": \"test command received!\"}");
        }
        

    }
}

void on_websocket_event(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        log_debug("New websocket connection received");
        client->text("{\"status\": \"ok\", \"message\": \"connection successfully established!\"}");
    }
    else if (type == WS_EVT_DATA) {
        char* message;
        message = (char*) malloc(len + 1);
        memcpy(message, data, len);
        message[len] = 0; // String terminator

        log_debug("Received data: %s", message);
        handle_websocket_data(message, client);

        free(message);
    }
    else if (type == WS_EVT_DISCONNECT) {
        log_debug("Websocket client disconnected");
    }
}


void init_websockets(AsyncWebServer* webserver) {
    /* This method must be called before $webserver.begin */
    ws.onEvent(on_websocket_event);
    webserver->addHandler(&ws);
}

void websocket_loop() {
    /* This method should be called at least every sixty seconds, more often is better */
    if (millis() - ws_last_cleanup_millis > 60000) { // just execute every 60 seconds..
        ws.cleanupClients();
        ws_last_cleanup_millis = millis();
        log_debug("Cleaned up websocket clients");
    }
}