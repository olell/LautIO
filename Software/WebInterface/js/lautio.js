/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

var AMP_AB = "AB";
var AMP_CD = "CD";

// control type defs
var DSP_CONTROL_VOLSLEW         =  0
var DSP_CONTROL_MUX             =  1
var DSP_CONTROL_EQ_SECOND_ORDER =  2
var DSP_CONTROL_DC_SOURCE       =  3
var DSP_CONTROL_SINE_SOURCE     =  4
var DSP_CONTROL_SQUARE_SOURCE   =  5
var DSP_CONTROL_SAWTOOTH_SOURCE =  6
var DSP_CONTROL_TRIANGLE_SOURCE =  7
var DSP_CONTROL_AUDIO_DELAY     =  8
var DSP_CONTROL_EQ_FIRST_ORDER  =  9
var DSP_CONTROL_GAIN            = 10
var DSP_CONTROL_DEMUX           = 11
var DSP_CONTROL_SOFT_CLIP       = 12
var DSP_CONTROL_HARD_CLIP       = 13
var DSP_CONTROL_COMPRESSOR_RMS  = 14
var DSP_CONTROL_COMPRESSOR_PEAK = 15
var DSP_CONTROL_TONE_CONTROL    = 16
var DSP_CONTROL_STATE_VARIABLE  = 17
var DSP_CONTROL_MUTE_ADC        = 18
var DSP_CONTROL_MUTE_DAC        = 19

var SOEQ_FILTER_TYPE_PEAKING             = 0
var SOEQ_FILTER_TYPE_PARAMETRIC          = 1
var SOEQ_FILTER_TYPE_LOW_SHELF           = 2
var SOEQ_FILTER_TYPE_HIGH_SHELF          = 3
var SOEQ_FILTER_TYPE_LOWPASS             = 4
var SOEQ_FILTER_TYPE_HIGHPASS            = 5
var SOEQ_FILTER_TYPE_BANDPASS            = 6
var SOEQ_FILTER_TYPE_BANDSTOP            = 7
var SOEQ_FILTER_TYPE_BUTTERWORTH_LOWPASS  = 8
var SOEQ_FILTER_TYPE_BUTTERWORTH_HIGHPASS = 9
var SOEQ_FILTER_TYPE_BESSEL_LOWPASS      = 10
var SOEQ_FILTER_TYPE_BESSEL_HIGHPASS     = 11

class LautIO {
    constructor(address) {
        this.address = address;

        this.connection_status_update_interval = 1000; // twice a minute
        
        this.connected = false;
        this.websocket = null;
        this._connection_update_interval = null;
        this.connection_status_callback = function(x) {};
        this._waiting_for_connection_status = null;
        this.connection_timeout = 2;

        this.ws_out_log_cb = null;
        this.ws_in_log_cb = null;

        this.dsp_controls = [];
        this.updated_controls_callback = function() {};

        this.amp_status = {"AB": undefined, "CD": undefined};
        this.updated_amp_status_callback = function() {};

        this.ui_config = {"name": undefined, "quick_controls": []}
        this.updated_ui_config_callback = function() {};

        this.dsp_config = {}
        this.updated_dsp_config_callback = function() {};

        this.led = false;
    }

    // getter/setter
    get_address()        { return this.address; }
    set_address(address) { this.address = address; }

    get_connection_status() { return this.connected; }

    set_connection_status_update_interval(milliseconds) {
        this.connection_status_update_interval = milliseconds;
        clearInterval(this._connection_update_interval);
        this._connection_update_interval = setInterval(this._update_connection_status.bind(this), this.connection_status_update_interval);
    }
    get_connection_status_update_interval() { return this.connection_status_update_interval; }

    set_connection_status_callback(callback) { this.connection_status_callback = callback; }
    set_updated_controls_callback(callback) { this.updated_controls_callback = callback; }
    set_updated_amp_status_callback(callback) { this.updated_amp_status_callback = callback; }
    set_updated_ui_config_callback(callback) { this.updated_ui_config_callback = callback; }
    

    
    // commands

    _send_lautio_command(category, command, data) {
        if (this.connected) {
            var send = {
                "category": category,
                "command": command,
                ...data
            };
            try {
                var payload = JSON.stringify(send);
                this.websocket.send(payload);
                this.ws_out_log_cb(payload);
            }
            catch (e) {
                // assuming connection loss
                console.log("e", e);
                this.connected = false;
                this.connection_status_callback(false);
            }
        }
    }

    _update_connection_status() {
        try {
            var payload = JSON.stringify({
                "category": "system",
                "command": "test_connection"
            });
            // console_log_wsout(payload); // do not log connection status update message
            this.websocket.send(payload);
            if (this._waiting_for_connection_status === null)
                this._waiting_for_connection_status = Date.now();
            if (Date.now() - this._waiting_for_connection_status > this.connection_timeout * 1000) {
                this.connection_status_callback(false);
            }
        }
        catch (e) {
            console.log(e);
            this.connected = false;
        }
    }
    
    update_dsp_controls() {
        this._send_lautio_command("dsp", "get_controls");
    }

    update_amp_status() {
        this._send_lautio_command("amp", "get_status");
    }

    set_led_state(state) {
        this.led = state;
        this._send_lautio_command("system", "led", {"state": this.led})
    }

    set_amp_reset_state(amp, state) {
        this._send_lautio_command("amp", "set_reset_state", {"amp": amp, "state": state})
        
    }

    update_control(control) {
        this._send_lautio_command("dsp", "update_control", {"control": control})
    }

    update_ui_config() {
        this._send_lautio_command("ui", "get_ui_config");
    }

    update_dsp_config() {
        this._send_lautio_command("config", "get_section", {"section": "dsp"})
    }

    reboot() {
        this._send_lautio_command("system", "restart");
    }
    
    // functions
    _ws_msg_handler(recv) {
        var message = JSON.parse(recv.data);
        
        if (message.cmd != "system_test_connection") this.ws_in_log_cb(recv.data);

        if (message.cmd == "system_test_connection") {
            var state = message.status == "ok";
            this.connected = state;
            this.connection_status_callback(state);
            this._waiting_for_connection_status = null;
        }

        else if (message.cmd == "dsp_controls") {
            if (message.status == "ok") {
                this.dsp_controls = message.controls;
                this.updated_controls_callback();
            }
        }

        else if (message.cmd == "amp_set_reset_state") {
            this.update_amp_status()
        }

        else if (message.cmd == "amp_status") {
            this.amp_status.AB = message.AB;
            this.amp_status.CD = message.CD;
            this.updated_amp_status_callback();
        }

        else if (message.cmd == "ui_config") {
            this.ui_config = message.config;
            this.updated_ui_config_callback();
        }

        else if (message.cmd == "config_get_section") {
            if (message.section == "dsp") {
                this.dsp_config = message["content"];
            }
        }

    }

    connect() {
        this.websocket = new WebSocket(`ws://${this.address}/ws`);
        this.websocket.onmessage = this._ws_msg_handler.bind(this);
        this.set_connection_status_update_interval(this.connection_status_update_interval);
        this._update_connection_status();
    }


}