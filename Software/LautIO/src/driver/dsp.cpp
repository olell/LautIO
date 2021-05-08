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

// dsp register size / addr vars
uint16_t cr0_size, cr0_addr;
uint16_t cr4_size, cr4_addr;
uint16_t program_size, program_addr;
uint16_t param_size, param_addr;
uint16_t hwconf_size, hwconf_addr;

uint8_t* cr0_data;
uint8_t* cr4_data;
uint8_t* program_data;
uint8_t* param_data;
uint8_t* hwconf_data;

SigmaDSP dsp(Wire, DSP_I2C_ADDRESS, 48000.00f, DSP_RESET_PIN);

DynamicJsonDocument dsp_controls(2048); // maybe to small..

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

    dsp_change_firmware((const char*) Configuration::section("dsp")["default_firmware"]);

}


bool dsp_ping() {
    // Ping one time and get success status
    return dsp.ping() == 0;
}

void dsp_load_data(const char* dirname) {
    log_info("Loading dsp data from directory %s", dirname);

    // loading sizes and addresses from register.json file
    char file_path[32]; // will be reused
    sprintf(file_path, "/dsp/%s/registers.json", dirname);
    File reg_file = FSHANDLE.open(file_path);
    StaticJsonDocument<512> register_json;
    DeserializationError e = deserializeJson(register_json, reg_file);
    if (e) {
        log_fatal("Failed to load registers.json from dsp firmware!");
        log_fatal(": %s", e.c_str());
        ESP.deepSleep(0);
    }
    cr0_addr = register_json["cr0"][0];
    cr0_size = register_json["cr0"][1];
    cr4_addr = register_json["cr4"][0];
    cr4_size = register_json["cr4"][1];
    param_addr = register_json["param"][0];
    param_size = register_json["param"][1];
    hwconf_addr = register_json["hwconf"][0];
    hwconf_size = register_json["hwconf"][1];
    program_addr = register_json["program"][0];
    program_size = register_json["program"][1];

    // free unused data
    reg_file.close();
    register_json.clear();
    
    log_debug("Loaded register size/addr file");
    log_debug("| REGISTER | ADDR | SIZE |");
    log_debug("|----------|------|------|");
    log_debug("|      CR0 | %4d | %4d |", cr0_addr, cr0_size);
    log_debug("|      CR4 | %4d | %4d |", cr4_addr, cr4_size);
    log_debug("|   HWCONF | %4d | %4d |", hwconf_addr, hwconf_size);
    log_debug("|    PARAM | %4d | %4d |", param_addr, param_size);
    log_debug("|  PROGRAM | %4d | %4d |", program_addr, program_size);

    // load register data from seperate files

    // allocate memory
    cr0_data = (uint8_t*) malloc(cr0_size);
    cr4_data = (uint8_t*) malloc(cr4_size);
    param_data = (uint8_t*) malloc(param_size);
    hwconf_data = (uint8_t*) malloc(hwconf_size);
    program_data = (uint8_t*) malloc(program_size);

    log_debug("Allocated memory for dsp firmware");

    // load program file
    sprintf(file_path, "/dsp/%s/program.bin", dirname);
    File program_file = FSHANDLE.open(file_path);
    uint16_t addr = 0;
    while(program_file.available()) {
        program_data[addr] = program_file.read();
        addr ++;
    }
    log_debug("Loaded program data");
    program_file.close();

    // load param file
    sprintf(file_path, "/dsp/%s/param.bin", dirname);
    File param_file = FSHANDLE.open(file_path);
    addr = 0;
    while(param_file.available()) {
        param_data[addr] = param_file.read();
        addr ++;
    }
    log_debug("Loaded param data");
    param_file.close();

    // load cr0 file
    sprintf(file_path, "/dsp/%s/cr0.bin", dirname);
    File cr0_file = FSHANDLE.open(file_path);
    addr = 0;
    while(cr0_file.available()) {
        cr0_data[addr] = cr0_file.read();
        addr ++;
    }
    log_debug("Loaded cr0 data");
    cr0_file.close();

    // load cr4 file
    sprintf(file_path, "/dsp/%s/cr4.bin", dirname);
    File cr4_file = FSHANDLE.open(file_path);
    addr = 0;
    while(cr4_file.available()) {
        cr4_data[addr] = cr4_file.read();
        addr ++;
    }
    log_debug("Loaded cr4 data");
    cr4_file.close();

    // load hwconf file
    sprintf(file_path, "/dsp/%s/hwconf.bin", dirname);
    File hwconf_file = FSHANDLE.open(file_path);
    addr = 0;
    while(hwconf_file.available()) {
        hwconf_data[addr] = hwconf_file.read();
        addr ++;
    }
    log_debug("Loaded hwconf data");
    hwconf_file.close();

}

void dsp_upload_firmware() {
    // upload firmware to dsp
    log_info("Uploading firmware to dsp");
    dsp.writeRegister(cr0_addr, cr0_size, cr0_data);
    log_debug("Uploaded CR0 data");
    dsp.writeRegisterBlock(program_addr, program_size, program_data, PROGRAM_REGSIZE);
    log_debug("Uploaded PROGRAM data");
    dsp.writeRegisterBlock(param_addr, param_size, param_data, PARAM_REGSIZE);
    log_debug("Uplaoded PARAM data");
    dsp.writeRegister(hwconf_addr, hwconf_size, hwconf_data);
    log_debug("Uploaded HWCONF data");
    dsp.writeRegister(cr4_addr, cr4_size, cr4_data);
    log_debug("Uploaded CR4 data, all done!");
    log_info("Uploaded data!");
}

void dsp_load_controls(const char* dirname) {
    log_info("Loading dsp controls");
    char filename[32];
    sprintf(filename, "/dsp/%s/controls.json", dirname);
    File controls_file = FSHANDLE.open(filename);
    DeserializationError e = deserializeJson(dsp_controls, controls_file);
    if (e) {
        log_fatal("Failed to load controls.. dsp controls will not be available!");
        log_debug("Fail reason: %s", e.c_str());
    }
    log_debug("loaded controls. %d controls found!", dsp_controls["controls"].size());
}

void dsp_change_firmware(const char* firmware_name) {
    // load data from default firmware
    dsp_load_data(firmware_name);
    // upload dsp firmware
    dsp_upload_firmware();
    // loading dsp controls
    dsp_load_controls(firmware_name);    
}

DynamicJsonDocument* get_controls_json() {
    return &dsp_controls;
}

JsonObject get_control_by_id(uint8_t ctrl_id) {
    for (int i = 0; i < dsp_controls["controls"].size(); i ++) {
        if ((uint8_t) dsp_controls["controls"][i]["id"] == ctrl_id) {
            return dsp_controls["controls"][i];
        }
    }
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

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["volume"] = volume;
            ctrl["slew"] = slew;
            
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

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["index"] = index;
            
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

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["Q"] = eq_param.Q;
            ctrl["S"] = eq_param.S;
            ctrl["bandwidth"] = eq_param.bandwidth;
            ctrl["boost"] = eq_param.boost;
            ctrl["freq"] = eq_param.freq;
            ctrl["gain"] = eq_param.gain;
            ctrl["filter_type"] = eq_param.filterType;
            ctrl["phase"] = eq_param.phase;
            ctrl["state"] = eq_param.state;
            
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
            log_debug("Setting DAC mute state to %s",
                mute ? "mute" : "unmute");

            dsp.muteDAC(mute);

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["mute"] = mute;
            
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
            log_debug("Setting ADC mute state to %s",
                mute ? "mute" : "unmute");

            dsp.muteADC(mute);

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["mute"] = mute;
            
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

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["level"] = level;
            
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

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["freq"] = freq;
            
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

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["freq"] = freq;
            
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

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["freq"] = freq;
            
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

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["freq"] = freq;
            
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

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["delay"] = delay_ms;
            
        }
    }
}

void dsp_ctrl_eq_first_order(JsonObject control, firstOrderEQ_t eq_param) {
    // updates the value of a first order eq control
    
    // required json fields
    // uint8   id          -> control id
    // uint16  addr        -> control addr
    // uint8   type        -> control type
    // float   freq        -> Range 20-20000 (Hz)
    // float   gain        -> Range +/-15 (dB)
    // uint8   filter_type -> parameters::filterType::*
    // uint8   phase       -> parameters::phase::*
    // uint8   state       -> parameters::state::*
    // bool    ro          -> read only, will skip overwrite if true
    // bool    change      -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_EQ_FIRST_ORDER) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            uint16_t control_addr = control["addr"];
            log_debug("Updating 1st order EQ %d @ %d",
                control_id, control_addr);

            dsp.EQfirstOrder(control_addr, eq_param);

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["freq"] = eq_param.freq;
            ctrl["gain"] = eq_param.gain;
            ctrl["filter_type"] = eq_param.filterType;
            ctrl["phase"] = eq_param.phase;
            ctrl["state"] = eq_param.state;
            
        }
    }
}

void dsp_ctrl_gain(JsonObject control, float gain) {
    // updates the value of a gain control
    
    // required json fields
    // uint8   id       -> control id
    // uint16  addr     -> control addr
    // uint8   type     -> control type
    // float   gain     -> gain (1-> 0dB 2 -> +6dB)
    // uint8   channels -> number of channels
    // bool    ro       -> read only, will skip overwrite if true
    // bool    change   -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_GAIN) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            uint16_t control_addr = control["addr"];
            uint8_t channels = control["channels"];
            log_debug("Updating gain control %d @ %d to %f",
                control_id, control_addr, gain);

            dsp.gain(control_addr, gain, channels);

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["gain"] = gain;
            
        }
    }
}

void dsp_ctrl_demux(JsonObject control, uint8_t index) {
    // updates the value of a demux control
    
    // required json fields
    // uint8   id      -> control id
    // uint16  addr    -> control addr
    // uint8   type    -> control type
    // uint8   index   -> demux index
    // uint8_t num_idx -> number of indexes
    // bool    ro      -> read only, will skip overwrite if true
    // bool    change  -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_DEMUX) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            uint16_t control_addr = control["addr"];
            uint8_t num_idx = control["num_idx"];
            log_debug("Updating demux %d @ %d to %d/%d",
                control_id, control_addr, index, num_idx);

            dsp.demux(control_addr, index, num_idx);

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["index"] = index;
            
        }
    }

}

void dsp_ctrl_soft_clip(JsonObject control, float alpha) {
    // updates the value of a soft clip control
    
    // required json fields
    // uint8   id      -> control id
    // uint16  addr    -> control addr
    // uint8   type    -> control type
    // float   alpha   -> clipping coefficient (0.1 ... 10.0)
    // bool    ro      -> read only, will skip overwrite if true
    // bool    change  -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_SOFT_CLIP) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            uint16_t control_addr = control["addr"];
            log_debug("Updating soft clip control %d @ %d to %f",
                control_id, control_addr, alpha);

            dsp.softClip(control_addr, alpha);

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["alpha"] = alpha;
            
        }
    }
}

void dsp_ctrl_hard_clip(JsonObject control, float high_threshold, float low_threshold) {
    // updates the value of a hard clip control
    
    // required json fields
    // uint8   id             -> control id
    // uint16  addr           -> control addr
    // uint8   type           -> control type
    // float   high_threshold -> high threshold (-1.0 ... 0.0)
    // float   low_threshold  -> low threshold (0.0 ... 1.0)
    // bool    ro             -> read only, will skip overwrite if true
    // bool    change         -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_HARD_CLIP) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            uint16_t control_addr = control["addr"];
            log_debug("Updating hard clip control %d @ %d to %f %f",
                control_id, control_addr, low_threshold, high_threshold);

            dsp.hardClip(control_addr, high_threshold, low_threshold);

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["high_threshold"] = high_threshold;
            ctrl["low_threshold"] = low_threshold;
            
        }
    }
}

void dsp_ctrl_compressor_RMS(JsonObject control, compressor_t compressor_param) {
    // updates the value of a compressor rms control
    
    // required json fields
    // uint8   id             -> control id
    // uint16  addr           -> control addr
    // uint8   type           -> control type
    // float   threshold      -> -90 / +6 dB
    // float   ratio          -> 1 ... 100
    // float   attack         -> 1 ... 500 ms
    // float   hold           -> 0 ... 500 ms
    // float   decay          -> 0 ... 2000 ms
    // float   postgain       -> -30 ... +24 dB
    // bool    ro             -> read only, will skip overwrite if true
    // bool    change         -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_COMPRESSOR_RMS) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            uint16_t control_addr = control["addr"];
            log_debug("Updating compressor rms control %d @ %d",
                control_id, control_addr);

            dsp.compressorRMS(control_addr, compressor_param);

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["threshold"] = compressor_param.threshold;
            ctrl["ratio"] = compressor_param.ratio;
            ctrl["attack"] = compressor_param.attack;
            ctrl["hold"] = compressor_param.hold;
            ctrl["decay"] = compressor_param.decayMs;
            ctrl["postgain"] = compressor_param.postgain;
            
        }
    }
}

void dsp_ctrl_compressor_peak(JsonObject control, compressor_t compressor_param) {
    // updates the value of a compressor peak control
    
    // required json fields
    // uint8   id             -> control id
    // uint16  addr           -> control addr
    // uint8   type           -> control type
    // float   threshold      -> -90 / +6 dB
    // float   ratio          -> 1 ... 100
    // float   attack         -> 1 ... 500 ms
    // float   hold           -> 0 ... 500 ms
    // float   decay          -> 0 ... 2000 ms
    // float   postgain       -> -30 ... +24 dB
    // bool    ro             -> read only, will skip overwrite if true
    // bool    change         -> change json to new value after updating control

    uint8_t control_type = control["type"];
    if (control_type == DSP_CONTROL_COMPRESSOR_PEAK) { // check if control type is correct

        bool read_only = control["ro"];
        if (!read_only) { // Do not update if readonly prop is true
            uint8_t control_id = control["id"];
            uint16_t control_addr = control["addr"];
            log_debug("Updating compressor peak control %d @ %d",
                control_id, control_addr);

            dsp.compressorPeak(control_addr, compressor_param);

            // write changes to local control object
            JsonObject ctrl = get_control_by_id(control["id"]);
            ctrl["threshold"] = compressor_param.threshold;
            ctrl["ratio"] = compressor_param.ratio;
            ctrl["attack"] = compressor_param.attack;
            ctrl["hold"] = compressor_param.hold;
            ctrl["decay"] = compressor_param.decayMs;
            ctrl["postgain"] = compressor_param.postgain;
            
        }
    }
}

void dsp_ctrl_tone_control(JsonObject control) {
    // todo
}

void dsp_ctrl_state_variable(JsonObject control) {
    // todo
}

void dsp_update_from_updated_json(JsonObject control) {
    // wrapper to simplify the update of any control
    uint8_t ctrl_type = control["type"];
    if (ctrl_type == DSP_CONTROL_VOLSLEW) {
        dsp_ctrl_volslew(control, (float) control["volume"], (uint8_t) control["slew"]);
    }
    else if (ctrl_type == DSP_CONTROL_MUX) {
        dsp_ctrl_mux(control, (uint8_t) control["index"]);
    }
    else if (ctrl_type == DSP_CONTROL_EQ_SECOND_ORDER) {
        secondOrderEQ_t param;
        param.Q = (float) control["Q"];
        param.S = (float) control["S"];
        param.bandwidth = (float) control["bandwidth"];
        param.freq = (float) control["freq"];
        param.gain = (float) control["gain"];
        param.filterType = (uint8_t) control["filter_type"];
        param.phase = (uint8_t) control["phase"];
        param.state = (uint8_t) control["state"];
        dsp_ctrl_eq_second_order(control, param);
    }
    else if (ctrl_type = DSP_CONTROL_MUTE_DAC) {
        dsp_ctrl_mute_dac(control, (bool) control["mute"]);
    }
    else if (ctrl_type = DSP_CONTROL_MUTE_ADC) {
        dsp_ctrl_mute_adc(control, (bool) control["mute"]);
    }
    else if (ctrl_type == DSP_CONTROL_DC_SOURCE) {
        dsp_ctrl_dc_source(control, (float) control["level"]);
    }
    else if (ctrl_type == DSP_CONTROL_SINE_SOURCE) {
        dsp_ctrl_sine_source(control, (float) control["freq"]);
    }
    else if (ctrl_type == DSP_CONTROL_SQUARE_SOURCE) {
        dsp_ctrl_square_source(control, (float) control["freq"]);
    }
    else if (ctrl_type == DSP_CONTROL_SAWTOOTH_SOURCE) {
        dsp_ctrl_sawtooth_source(control, (float) control["freq"]);
    }
    else if (ctrl_type == DSP_CONTROL_TRIANGLE_SOURCE) {
        dsp_ctrl_triangle_source(control, (float) control["freq"]);
    }
    else if (ctrl_type == DSP_CONTROL_AUDIO_DELAY) {
        dsp_ctrl_audio_delay(control, (float) control["delay"]);
    }
    else if (ctrl_type == DSP_CONTROL_EQ_FIRST_ORDER) {
        firstOrderEQ_t param;
        param.freq = (float) control["freq"];
        param.gain = (float) control["gain"];
        param.filterType = (uint8_t) control["filter_type"];
        param.phase = (uint8_t) control["phase"];
        param.state = (uint8_t) control["state"];
        dsp_ctrl_eq_first_order(control, param);
    }
    else if (ctrl_type == DSP_CONTROL_GAIN) {
        dsp_ctrl_gain(control, (float) control["gain"]);
    }
    else if (ctrl_type == DSP_CONTROL_DEMUX) {
        dsp_ctrl_demux(control, (uint8_t) control["index"]);
    }
    else if (ctrl_type == DSP_CONTROL_SOFT_CLIP) {
        dsp_ctrl_soft_clip(control, (float) control["alpha"]);
    }
    else if (ctrl_type == DSP_CONTROL_HARD_CLIP) {
        dsp_ctrl_hard_clip(control, (float) control["high_threshold"], (float) control["low_threshold"]);
    }
    else if (ctrl_type == DSP_CONTROL_COMPRESSOR_RMS) {
        // todo
    }
    else if (ctrl_type == DSP_CONTROL_COMPRESSOR_PEAK) {
        // todo
    }
    else if (ctrl_type == DSP_CONTROL_TONE_CONTROL) {
        // todo
    }
    else if (ctrl_type == DSP_CONTROL_STATE_VARIABLE) {
        // todo
    }
}