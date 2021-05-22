/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

class LautIO {
    constructor(address) {
        this.address = address;

        this.connection_status_update_interval = 3000; // twice a minute
        
        this.connected = false;
        this.websocket = null;
        this._connection_update_interval = null;
        this.connection_status_callback = function(x) {};

        this.dsp_controls = [];
        this.updated_controls_callback = function() {};

        this.amp_status = {"AB": undefined, "CD": undefined};
        this.updated_amp_status_callback = function() {};
    }

    // getter/setter
    get_address()        { return this.address; }
    set_address(address) { this.address = address; }

    get_connection_status() { return this.connected; }

    set_connection_status_update_interval(milliseconds) {
        this.connection_status_update_interval = milliseconds;
        if (this._connection_update_interval) {
            clearInterval(this._connection_update_interval);
            this._connection_update_interval = setInterval(this._update_connection_status.bind(this), this.connection_status_update_interval);
        }
    }
    get_connection_status_update_interval() { return this.connection_status_update_interval; }

    set_connection_status_callback(callback) { this.connection_status_callback = callback; }
    set_updated_controls_callback(callback) { this.updated_controls_callback = callback; }
    set_updated_amp_status_callback(callback) { this.updated_amp_status_callback = callback; }
    

    
    // commands

    _send_lautio_command(category, command, data) {
        if (this.connected) {
            var send = {
                "category": category,
                "command": command,
                ...data
            };
            try {
                this.websocket.send(JSON.stringify(send));
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
            this.websocket.send(JSON.stringify({
                "category": "system",
                "command": "test_connection"
            }));
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
    
    // functions
    _ws_msg_handler(recv) {
        var message = JSON.parse(recv.data);

        if (message.cmd == "system_test_connection") {
            var state = message.status == "ok";
            this.connected = state;
            this.connection_status_callback(state);
        }

        else if (message.cmd == "dsp_controls") {
            if (message.status == "ok") {
                this.dsp_controls = message.controls;
                this.updated_controls_callback();
            }
        }

        else if (message.cmd == "amp_status") {
            this.amp_status.AB = message.AB;
            this.amp_status.CD = message.CD;
            this.updated_amp_status_callback();
        }
    }

    connect() {
        this.websocket = new WebSocket(`ws://${this.address}/ws`);
        this.websocket.onmessage = this._ws_msg_handler.bind(this);
        this._update_connection_status();
        this._connection_update_interval = setInterval(this._update_connection_status.bind(this), this.connection_status_update_interval);
    }


}