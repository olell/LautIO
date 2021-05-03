/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */


#include "sys_config.h"

// utils
#include "util/config.h"

// driver
#include "driver/filesystem.h"

Configuration* Configuration::_instance = 0;

Configuration* Configuration::get_instance() {
    // Get the instance from this singleton class
    if (!Configuration::_instance)
        Configuration::_instance = new Configuration();
    return Configuration::_instance;
}

Configuration::Configuration() {
    // Configuration constructor

}