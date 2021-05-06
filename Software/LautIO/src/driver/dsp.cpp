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
    // uint8   id     -> control id
    // uint16  addr   -> control addr
    // uint8   type   -> control type
    // float   volume -> current volume
    // uint8   slew   -> current slew
    // bool    ro     -> read only, will skip overwrite if true
    // bool    change -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_VOLSLEW) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            uint16_t control_addr = control["addr"];
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

void dsp_ctrl_mux(JsonObject control, uint8_t index) {
    // updates the value of a mux control
    
    // required json fields
    // uint8   id      -> control id
    // uint16  addr    -> control addr
    // uint8   type    -> control type
    // uint8   index   -> mux index
    // bool    ro     -> read only, will skip overwrite if true
    // bool    change -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_MUX) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            uint16_t control_addr = control["addr"];
            log_debug("Updating mux %d @ %d to %d",
                control_id, control_addr, index);

            dsp.mux(control_addr, index);

            // write changes to control object
            // Change prop will be interpreted on write to some file
            control["index"] = index;
            
        }
    }

}

void dsp_ctrl_eq_second_order(JsonObject control, secondOrderEQ_t eq_param) {
    // updates the value of a second order eq control
    
    // required json fields
    // uint8   id          -> control id
    // uint16  addr        -> control addr
    // uint8   type        -> control type
    // float   Q           -> range 0-16 (for filter types parametric & peaking)
    // float   S           -> Slope, range 0-12 (for filter types lowShelf & highShelf)
    // float   bandwidth   -> Bandwidth in octaves (for filter types bandPass & bandStop)
    // float   boost       -> Range +/-15 (dB)
    // float   freq        -> Range 20-20000 (Hz)
    // float   gain        -> Range +/-15 (dB)
    // uint8   filter_type -> parameters::filterType::*
    // uint8   phase       -> parameters::phase::*
    // uint8   state       -> parameters::state::*
    // bool    ro          -> read only, will skip overwrite if true
    // bool    change      -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_EQ_SECOND_ORDER) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            uint16_t control_addr = control["addr"];
            log_debug("Updating 2nd order EQ %d @ %d",
                control_id, control_addr);

            dsp.EQsecondOrder(control_addr, eq_param);

            // write changes to control object
            // Change prop will be interpreted on write to some file
            control["Q"] = eq_param.Q;
            control["S"] = eq_param.S;
            control["bandwidth"] = eq_param.bandwidth;
            control["boost"] = eq_param.boost;
            control["freq"] = eq_param.freq;
            control["gain"] = eq_param.gain;
            control["filter_type"] = eq_param.filterType;
            control["phase"] = eq_param.phase;
            control["state"] = eq_param.state;
            
        }
    }
}

void dsp_ctrl_mute_dac(JsonObject control, bool mute) {
    // updates the value of the dac mute
    
    // required json fields
    // uint8   id      -> control id
    // uint8   type    -> control type
    // bool    mute    -> mute state
    // bool    ro      -> read only, will skip overwrite if true
    // bool    change  -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_MUTE_DAC) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            log_debug("Setting DAC mute state to %s",
                mute ? "mute" : "unmute");

            dsp.muteDAC(mute);

            // write changes to control object
            // Change prop will be interpreted on write to some file
            control["mute"] = mute;
            
        }
    }
}

void dsp_ctrl_mute_adc(JsonObject control, bool mute) {
    // updates the value of the adc mute
    
    // required json fields
    // uint8   id      -> control id
    // uint8   type    -> control type
    // bool    mute    -> mute state
    // bool    ro      -> read only, will skip overwrite if true
    // bool    change  -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_MUTE_ADC) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            log_debug("Setting ADC mute state to %s",
                mute ? "mute" : "unmute");

            dsp.muteADC(mute);

            // write changes to control object
            // Change prop will be interpreted on write to some file
            control["mute"] = mute;
            
        }
    }
}

void dsp_ctrl_dc_source(JsonObject control, float level) {
    // updates the value of a dc source control
    
    // required json fields
    // uint8   id      -> control id
    // uint16  addr    -> control addr
    // uint8   type    -> control type
    // float   level   -> dc source level
    // bool    ro      -> read only, will skip overwrite if true
    // bool    change  -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_DC_SOURCE) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            uint16_t control_addr = control["addr"];
            log_debug("Updating DC control %d @ %d to %f",
                control_id, control_addr, level);

            dsp.dcSource(control_addr, level);

            // write changes to control object
            // Change prop will be interpreted on write to some file
            control["level"] = level;
            
        }
    }
}

void dsp_ctrl_sine_source(JsonObject control, float freq) {
    // updates the value of a sine source control
    
    // required json fields
    // uint8   id      -> control id
    // uint16  addr    -> control addr
    // uint8   type    -> control type
    // float   freq    -> frequency
    // bool    ro      -> read only, will skip overwrite if true
    // bool    change  -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_SINE_SOURCE) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            uint16_t control_addr = control["addr"];
            log_debug("Updating sine source control %d @ %d to %f",
                control_id, control_addr, freq);

            dsp.sineSource(control_addr, freq);

            // write changes to control object
            // Change prop will be interpreted on write to some file
            control["freq"] = freq;
            
        }
    }
}

void dsp_ctrl_square_source(JsonObject control, float freq) {
    // updates the value of a square source control
    
    // required json fields
    // uint8   id      -> control id
    // uint16  addr    -> control addr
    // uint8   type    -> control type
    // float   freq    -> frequency
    // bool    ro      -> read only, will skip overwrite if true
    // bool    change  -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_SQUARE_SOURCE) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            uint16_t control_addr = control["addr"];
            log_debug("Updating square source control %d @ %d to %f",
                control_id, control_addr, freq);

            dsp.squareSource(control_addr, freq);

            // write changes to control object
            // Change prop will be interpreted on write to some file
            control["freq"] = freq;
            
        }
    }
}

void dsp_ctrl_sawtooth_source(JsonObject control, float freq) {
    // updates the value of a sawtooth source control
    
    // required json fields
    // uint8   id      -> control id
    // uint16  addr    -> control addr
    // uint8   type    -> control type
    // float   freq    -> frequency
    // bool    ro      -> read only, will skip overwrite if true
    // bool    change  -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_SAWTOOTH_SOURCE) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            uint16_t control_addr = control["addr"];
            log_debug("Updating sawtooth source control %d @ %d to %f",
                control_id, control_addr, freq);

            dsp.sawtoothSource(control_addr, freq);

            // write changes to control object
            // Change prop will be interpreted on write to some file
            control["freq"] = freq;
            
        }
    }
}

void dsp_ctrl_triangle_source(JsonObject control, float freq) {
    // updates the value of a triangle source control
    
    // required json fields
    // uint8   id      -> control id
    // uint16  addr    -> control addr
    // uint8   type    -> control type
    // float   freq    -> frequency
    // bool    ro      -> read only, will skip overwrite if true
    // bool    change  -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_TRIANGLE_SOURCE) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            uint16_t control_addr = control["addr"];
            log_debug("Updating triangle source control %d @ %d to %f",
                control_id, control_addr, freq);

            dsp.triangleSource(control_addr, freq);

            // write changes to control object
            // Change prop will be interpreted on write to some file
            control["freq"] = freq;
            
        }
    }
}

void dsp_ctrl_audio_delay(JsonObject control, float delay_ms) {
    // updates the value of an audio delay control
    
    // required json fields
    // uint8   id      -> control id
    // uint16  addr    -> control addr
    // uint8   type    -> control type
    // float   delay   -> delay in ms
    // bool    ro      -> read only, will skip overwrite if true
    // bool    change  -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_AUDIO_DELAY) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            uint16_t control_addr = control["addr"];
            log_debug("Updating audio delay control %d @ %d to %f ms",
                control_id, control_addr, delay_ms);

            dsp.audioDelay(control_addr, delay_ms);

            // write changes to control object
            // Change prop will be interpreted on write to some file
            control["dela"] = delay_ms;
            
        }
    }
}

void dsp_ctrl_eq_first_order(JsonObject control) {
    // todo
}

void dsp_ctrl_gain(JsonObject control) {
    // todo
}

void dsp_ctrl_demux(JsonObject control) {
    // todo
}

void dsp_ctrl_soft_clip(JsonObject control) {
    // todo
}

void dsp_ctrl_hard_clip(JsonObject control) {
    // todo
}

void dsp_ctrl_compressor_RMS(JsonObject control) {
    // todo
}

void dsp_ctrl_compressor_peak(JsonObject control) {
    // todo
}

void dsp_ctrl_tone_control(JsonObject control) {
    // todo
}

void dsp_ctrl_state_variable(JsonObject control) {
    // todo
}