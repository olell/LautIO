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

using SendAllHandler = void (*) (DynamicJsonDocument);
void send_all(DynamicJsonDocument data);
void register_send_all_handler(SendAllHandler h);