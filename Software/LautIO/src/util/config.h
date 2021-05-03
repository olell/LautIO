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
#include "ArduinoJson.h"

class Configuration {
    /*
    Config singleton, use the get method to obtain the instance of this class
    */
    
    public:
        static Configuration* get_instance();

        static JsonObject section(const char* key);

        static void write();

    private:
        static Configuration* _instance;
        
        DynamicJsonDocument json_doc;

        Configuration (); // private constructor to prevent creation of instances from other code

};
