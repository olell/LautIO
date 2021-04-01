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

void init_wifi();

void write_credentials_to_fs();
bool wifi_set_credentials(const char* ssid, const char* pass);