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


class Configuration {
    /*
    Config singleton, use the get method to obtain the instance of this class
    */
    
    public:
        static Configuration* get();

    private:
        static Configuration* _instance;
        Configuration (); // private constructor to prevent creation of instances from other code

};
