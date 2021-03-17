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

// Loglevels
#define LOGLEVEL_DEBUG 0
#define LOGLEVEL_INFO  1
#define LOGLEVEL_WARN  2
#define LOGLEVEL_ERROR 3
#define LOGLEVEL_FATAL 4
#define LOGLEVEL_OFF   5

void init_logging();

void log_debug(const char* val, ...);
void log_info(const char* val, ...);
void log_warn(const char* val, ...);
void log_error(const char* val, ...);
void log_fatal(const char* val, ...);
