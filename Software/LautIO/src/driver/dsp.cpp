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

#include "SigmaDSP.h"
#include <Wire.h>
#include "ArduinoJson.h"

// util
#include "util/log.h"
#include "util/config.h"

// driver
#include "driver/dsp.h"
#include "driver/filesystem.h" // to load firmware from fs


SigmaDSP dsp(Wire, DSP_I2C_ADDRESS, 48000.00f, DSP_RESET_PIN);

void init_dsp() {
    // initialise dsp
    log_info("Initialising DSP");

    Wire.begin();
    log_debug("Started wire");
    dsp.begin();
    log_debug("Started dsp");

    log_debug("Waiting a sec for DSP to come up");
    delay(1000);

    if (dsp_ping()) {
        log_info("DSP ping successful!");
    }
    else {
        log_fatal("Failed to ping DSP, please restart");
        ESP.deepSleep(0); // Put into deepsleep, basically halt
    }

    dsp_load_firmware();
}

bool dsp_ping() {
    // Ping one time and get success status
    return dsp.ping() == 0;
}

void dsp_load_firmware() {
    // upload firmware to dsp
}

/*
DSP Control functions
todo: currently in dev order, place later in meaningful order
*/

void dsp_ctrl_volslew(JsonObject control, float volume, uint8_t slew) {
    // Updates the value of an volume/slew control

    // required json fields
    // id     -> control id
    // addr   -> control addr
    // type   -> control type
    // volume -> current volume
    // slew   -> current slew
    // ro     -> read only, will skip overwrite if true
    // change -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_VOLSLEW) { // check if control type is correct
        uint8_t control_id = control["id"];
        uint16_t control_addr = control["addr"];

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            log_debug("Updating volslew %d @ %d to %.02fdB with %d slew",
                control_id, control_addr, volume, slew);

            dsp.volume_slew(control_addr, volume, slew);

            // write changes to control object
            // Change prop will be interpreted on write to some file
            control["volume"] = volume;
            control["slew"] = slew;
            
        }
    }

}

void dsp_ctrl_mux() {
    // todo
}

void dsp_ctrl_eq_second_order() {
    // todo
}

void dsp_ctrl_mute_dac() {
    // todo
}

void dsp_ctrl_mute_adc() {
    // todo
}

void dsp_ctrl_dc_source() {
    // todo
}

void dsp_ctrl_sine_source() {
    // todo
}

void dsp_ctrl_square_source() {
    // todo
}

void dsp_ctrl_sawtooth_source() {
    // todo
}

void dsp_ctrl_triangle_source() {
    // todo
}

void dsp_ctrl_audio_delay() {
    // todo
}

void dsp_ctrl_eq_first_order() {
    // todo
}

void dsp_ctrl_gain() {
    // todo
}

void dsp_ctrl_demux() {
    // todo
}

void dsp_ctrl_soft_clip() {
    // todo
}

void dsp_ctrl_hard_clip() {
    // todo
}

void dsp_ctrl_compressor_RMS() {
    // todo
}

void dsp_ctrl_compressor_peak() {
    // todo
}

void dsp_ctrl_tone_control() {
    // todo
}

void dsp_ctrl_state_variable() {
    // todo
}