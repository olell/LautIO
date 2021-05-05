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

#include "Arduino.h"

#include "sys_config.h"
#include "driver/filesystem.h"
#include "util/config.h"
#include "util/log.h"
#include "FTPServer.h"

FTPServer ftp_server(FSHANDLE);

void init_ftp_server() {
    // Starts the ftp server if enabled.. cannot be called before SD and WiFi init
    bool enable = Configuration::section("ftp")["enable"];
    if (enable) {

        const char* username = Configuration::section("ftp")["username"];
        const char* password = Configuration::section("ftp")["password"];

        log_info("Initialising FTP Server");
        ftp_server.begin(username, password);
        log_info("FTP Server started!");
    }
    else {
        log_warn("FTP Server disabled");
    }
}

void ftp_server_loop() {
    // todo: maybe find an asynchronous solution
    ftp_server.handleFTP();
}