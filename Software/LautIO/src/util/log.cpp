/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#include <Arduino.h>
#include <stdint.h>
#include <stdarg.h>

#include "util/log.h"
#include "config.h"

// ANSI Escape Sequence colors
const char* ANSI_BLACK   = "\u001b[30m";
const char* ANSI_RED     = "\u001b[31m";
const char* ANSI_GREEN   = "\u001b[32m";
const char* ANSI_YELLOW  = "\u001b[33m";
const char* ANSI_BLUE    = "\u001b[34m";
const char* ANSI_MAGENTA = "\u001b[35m";
const char* ANSI_CYAN    = "\u001b[36m";
const char* ANSI_WHITE   = "\u001b[37m";
const char* ANSI_RESET   = "\u001b[0m";

char log_output_str[150];


void init_logging() {
    Serial.begin(LOG_SERIAL_BAUD);
    log_info("Inited Logging");
}

void log_debug(const char* val, ...) {
    #if LOGLEVEL <= LOGLEVEL_DEBUG
        va_list args;
        va_start(args, val);
        vsprintf(log_output_str, val, args);
        Serial.printf("[%sDEBUG%s]   %s%10lu%s - %s\r\n", ANSI_GREEN, ANSI_RESET, ANSI_CYAN, millis(), ANSI_RESET, log_output_str);
    #endif
}
void log_info(const char* val, ...) {
    #if LOGLEVEL <= LOGLEVEL_INFO
        va_list args;
        va_start(args, val);
        vsprintf(log_output_str, val, args);
        Serial.printf("[%sINFO%s]    %s%10lu%s - %s\r\n", ANSI_BLUE, ANSI_RESET, ANSI_CYAN, millis(), ANSI_RESET, log_output_str);
    #endif
}
void log_warn(const char* val, ...) {
    #if LOGLEVEL <= LOGLEVEL_WARN
        va_list args;
        va_start(args, val);
        vsprintf(log_output_str, val, args);
        Serial.printf("[%sWARNING%s] %s%10lu%s - %s\r\n", ANSI_YELLOW, ANSI_RESET, ANSI_CYAN, millis(), ANSI_RESET, log_output_str);
    #endif
}
void log_error(const char* val, ...) {
    #if LOGLEVEL <= LOGLEVEL_ERROR
        va_list args;
        va_start(args, val);
        vsprintf(log_output_str, val, args);
        Serial.printf("[%sERROR%s]   %s%10lu%s - %s\r\n", ANSI_RED, ANSI_RESET, ANSI_CYAN, millis(), ANSI_RESET, log_output_str);
    #endif
}
void log_fatal(const char* val, ...) {
    #if LOGLEVEL <= LOGLEVEL_FATAL
        va_list args;
        va_start(args, val);
        vsprintf(log_output_str, val, args);
        Serial.printf("[%sFATAL%s]   %s%10lu%s - %s\r\n", ANSI_MAGENTA, ANSI_RESET, ANSI_CYAN, millis(), ANSI_RESET, log_output_str);
    #endif
}