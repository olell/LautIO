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

#include <ESPAsyncWebServer.h>

void init_websocket(AsyncWebServer* webserver);
void websocket_loop();