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

#include "SigmaDSP_parameters.h"

SigmaDSP dsp(Wire, DSP_I2C_ADDRESS, 48000.00f, DSP_RESET_PIN);

void dsp_upload_program() {
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

    log_debug("Uploading dsp program...");
    dsp_upload_program();
    log_info("DSP running!");

}

/* controls */
void dsp_update_volslew(uint8_t id, float volume, uint8_t slew) {
    uint16_t addr = 24; // todo read this from controls.csv
    dsp.volume_slew(addr, volume, slew);
}