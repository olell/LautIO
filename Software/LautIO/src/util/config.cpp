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

#include "sys_config.h"

// utils
#include "util/config.h"
#include "util/log.h"

// driver
#include "driver/filesystem.h"

Configuration* Configuration::_instance = 0;

Configuration* Configuration::get_instance() {
    // Get the instance from this singleton class
    if (!Configuration::_instance)
        Configuration::_instance = new Configuration();
    return Configuration::_instance;
}

Configuration::Configuration() : json_doc(8192) {
    // Configuration constructor

    log_info("Loading configuration");

    File config_file = FSHANDLE.open(CONFIG_FILE_PATH);

    DeserializationError error = deserializeJson(json_doc, config_file);
    if (error) {
        log_fatal("Failed to read configuration.. please restart controller!");
        ESP.deepSleep(0);
    }

    log_debug("Successfully loaded configuration");

    config_file.close();

}

JsonObject Configuration::section(const char* key) {
    // Obtain some value from the json document
    JsonObject obj = Configuration::get_instance()->json_doc[key];
    return obj;
}

void Configuration::write() {
    // Write configuration back to FS
    FSHANDLE.remove(CONFIG_FILE_PATH);

    File config_file = FSHANDLE.open(CONFIG_FILE_PATH, FILE_WRITE);
    if (!config_file) {
        // Old configuration file was removed to not append it + new file could not be created
        // Do not stop controller, imagine this happens while an live act :O
        log_error("Failed to create config file on sd card! Current configuration stays loaded until next reboot");
        return;
    }

    if (serializeJson(Configuration::get_instance()->json_doc, config_file) == 0) {
        // Some error occured while writing the file
        log_error("Failed to write updated configuration to file.. changes will stay until next reboot");
    }

    log_debug("Config sucessfully written!");

    config_file.close();

}