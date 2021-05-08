/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#include "AsyncWebSocket.h"
#include "ArduinoJson.h"

// util
#include "util/websockets.h"
#include "util/log.h"
#include "util/interfaces/interfaces.h"

AsyncWebSocket ws("/ws");
unsigned long ws_last_cleanup_millis = 0;

void handle_websocket_data(const char* message, AsyncWebSocketClient* client) {
    /* This method handles all incoming websocket data (on websocket 0.0.0.0/ws) */
    log_debug("Handling websocket data");
    DynamicJsonDocument json_document(1024);
    log_debug("Created json doc");
    DeserializationError e = deserializeJson(json_document, message);
    if (e) {
        log_debug("Websocket json parsing error %s!", e.c_str());
        client->text("JSON Error!");
        return;
    }
    log_debug("parsed json");
    client->text(handle_interfaces(json_document));
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

void init_websocket(AsyncWebServer* webserver) {
    ws.onEvent(on_websocket_event);
    webserver->addHandler(&ws);
    log_debug("Added websocket handler to webserver!");
}

void websocket_loop() {
    // cleanup orphaned websocket every minute
    if (millis() - ws_last_cleanup_millis > 60000) {
        ws.cleanupClients();
        ws_last_cleanup_millis = millis();
        log_debug("Cleaned up websocket clients!");
    }
}