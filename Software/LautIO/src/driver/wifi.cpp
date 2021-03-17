/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#include "Arduino.h"
#include "WiFi.h"

#include "config.h"
#include "util/log.h"

#if WIFI_BEHAVIOR == WIFI_AP_ONLY || WIFI_BEHAVIOR == WIFI_AP_CONFIG
    #include "DNSServer.h"
#endif

IPAddress local_ip(WIFI_AP_IP);
IPAddress gateway_ip(WIFI_AP_IP);
IPAddress subnet_mask(255, 255, 255, 0);

const char* ap_ssid = WIFI_AP_SSID;
const char* ap_pass = WIFI_AP_PASS;
char wifi_ssid[64] = WIFI_SSID;
char wifi_pass[64] = WIFI_PASS;

#if WIFI_BEHAVIOR == WIFI_AP_ONLY || WIFI_BEHAVIOR == WIFI_AP_CONFIG
    DNSServer dns_server;
#endif

bool wifi_set_credentials(const char* ssid, const char* pass) {
    if (sizeof(ssid) / sizeof(char) > 64 || sizeof(pass) / sizeof(char) > 64) return false;

    memset(wifi_ssid, 0, 64);
    memset(wifi_pass, 0, 64);

    strcpy(wifi_ssid, ssid);
    strcpy(wifi_pass, pass);

    log_debug("Updated WiFi Credentials.");

    return true;
}

#ifdef WIFI_BEHAVIOR == WIFI_CONFIG_CRED || WIFI_AP_CONFIG
bool wifi_connect() {
    log_info("Starting to connect to network \"%s\"", wifi_ssid);

    WiFi.mode(WIFI_STA);
    WiFi.enableAP(false);
    WiFi.begin(wifi_ssid, wifi_pass);
    WiFi.setSleep(false);
    WiFi.setHostname(HOSTNAME);

    bool connected = false;

    uint8_t current_status = 0;

    long t_start = millis();
    long t_now = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(10);

        current_status = WiFi.status();
        if (current_status == WL_DISCONNECTED) {
            // Do nothing, just waiting for connection

            t_now = millis();
            if (t_now - t_start > WIFI_CONNECT_TIMEOUT) {
                log_warn("Couldn't connect.. connection timeout reached!");
                connected = false;
                break;
            }

        }
        else if (current_status == WL_CONNECTED) {
            log_info("Connection successfully established!");
            log_info("My IP address is: %s", WiFi.localIP().toString().c_str());
            connected = true;
            break;
        }
        else if (current_status == WL_NO_SSID_AVAIL) {
            log_warn("SSID \"%s\" not found...", wifi_ssid);
            connected = false;
            break;
        }
        else if (current_status == WL_CONNECT_FAILED) {
            log_warn("WiFi connection failed, maybe invalid credentials?");
            connected = false;
            break;
        }
    }

    return connected;
}
#endif

#if WIFI_BEHAVIOR == WIFI_AP_ONLY || WIFI_BEHAVIOR == WIFI_AP_CONFIG
void init_access_point() {
    log_info("Starting AP");

    WiFi.mode(WIFI_AP);
    WiFi.enableAP(true);
    WiFi.softAP(ap_ssid, ap_pass);
    
    log_info("Started accesspoint, waiting a bit");
    delay(2000);
    log_info("Accesspoint should be ready soon");

    WiFi.softAPConfig(local_ip, gateway_ip, subnet_mask);
    log_debug("AP started and configurated!");

    dns_server.start(DNS_PORT, "*", local_ip);
    log_info("DNS Server started!");
}
#endif


void init_wifi() {
    log_debug("Begin wifi init");

    #if WIFI_BEHAVIOR == WIFI_AP_ONLY
    
    init_access_point();

    #elif WIFI_BEHAVIOR == WIFI_AP_CONFIG

    // TODO: load cred etc..

    #elif WIFI_BEHAVIOR == WIFI_CONFIG_CRED

    connect_to_network();

    #endif
}