/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#include "ArduinoJson.h"
#include "util/interfaces/com.h"

SendAllHandler handlers[5]; // 5 handlers max?
uint8_t handlers_count = 0;

void register_send_all_handler(SendAllHandler h) {
    handlers[handlers_count] = h;
    handlers_count ++;
}

void send_all(DynamicJsonDocument data) {
    size_t length = measureJson(data) + 1;
    char* text = (char*) malloc(length);
    serializeJson(data, text, length);
    send_all(text);
}

void send_all(const char* text) {
    for (int i = 0; i < handlers_count; i ++)
        handlers[i](text);
}
