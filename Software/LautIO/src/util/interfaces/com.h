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

using SendAllHandler = void (*) (const char*);
void send_all(DynamicJsonDocument data);
void send_all(const char* text);
void register_send_all_handler(SendAllHandler h);