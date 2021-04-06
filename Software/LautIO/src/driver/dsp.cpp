/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#include "config.h"

#include <Wire.h>
#include <SigmaDSP.h>
#include <ArduinoJson.h>

#include "driver/filesystem.h"
#include "SigmaDSP_parameters.h"

#include "driver/dsp.h"

SigmaDSP dsp(Wire, DSP_I2C_ADDRESS, 48000.00f, DSP_RESET_PIN);

DynamicJsonDocument controls_json(1024);
uint8_t ctrl_amount;

unsigned long last_controls_update = 0;

void update_controls_json() {
    if (millis() - last_controls_update > 60000) { // limit to one update every minute to decrease writes
        last_controls_update = millis();
        log_debug("Writing controls json to filesystem");
        File file = FSHANDLE.open("/dsp/controls.json", FILE_WRITE);
        serializeJson(controls_json, file);
        file.close();
    }
}

void load_controls() {
    log_debug("Loading controls json from filesystem");
    File file = FSHANDLE.open("/dsp/controls.json", FILE_READ);
    deserializeJson(controls_json, file);
    file.close();

    if (controls_json.containsKey("controls")) {
        ctrl_amount = controls_json["controls"].size();
        log_debug("Found %d configured controls", ctrl_amount);
    }

}

JsonObject get_control(uint8_t id) {
    JsonObject ctrl;
    for (int i = 0; i < ctrl_amount; i ++){
        if (controls_json["controls"][i].containsKey("id")) {
            ctrl = controls_json["controls"][i];
            uint8_t ctrl_id = ctrl["id"];
            if (ctrl_id == id){
                return ctrl;
            }
        }
    }
    return ctrl;
}

void dsp_upload_program() {
    log_debug("Uploading dsp program...");
    loadProgram(dsp);
}

void init_dsp() {
    log_debug("Begin DSP init");

    Wire.begin();
    dsp.begin();

    log_debug("Waiting two seconds for DSP to come up");
    delay(2000);

    log_info("Pinging dsp...");
    uint8_t dsp_response = dsp.ping();

    if (dsp_response == 0) {
        log_info("DSP pinged successfully!");
    }
    else {
        log_error("DSP ping unsuccessful! Response is %d", dsp_response);
    }

    dsp_upload_program();
    log_info("DSP running!");

    load_controls();
}

/* controls */
void dsp_update_volslew(uint8_t id, float volume, uint8_t slew) {
    JsonObject ctrl = get_control(id);
    //uint8_t slew = ctrl["slew"];
    log_debug("Read slew value %d", slew);
    if ((uint8_t) ctrl["type"] == CTRL_VOLSLEW) {
        uint16_t addr = ctrl["addr"];
        log_debug("Updating volslew %d @ %d to %.02fdB with %d slew", id, addr, volume, slew);
        dsp.volume_slew(addr, volume, slew);
        if (ctrl["const"] == false) {
            ctrl["volume"] = volume;
            ctrl["slew"] = slew;
            update_controls_json();
        }
    }
}