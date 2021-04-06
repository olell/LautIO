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
#include "AsyncWebSocket.h"
#include "ArduinoJson.h"
void http_dsp_setup_routes(AsyncWebServer* server);

void websocket_dsp_handle_command(DynamicJsonDocument json_doc, AsyncWebSocketClient* client);