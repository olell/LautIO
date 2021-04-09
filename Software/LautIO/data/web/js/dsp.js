/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

// Control type ids
var CTRL_VOLUME_SLEW = 0;
var CTRL_SECOND_ORDER_EQ = 1;

function generate_container(args, callback) {
    render_template("/static/tmpl/container.html", args,
    function (result) {
        callback(result);
    });
} 

function generate_control(path, args, callback) {
    render_template(path, args, function (result) {
        args.controls = result;
        generate_container(args, callback);
    });
}

function load_dsp_structure(cb) {
    $.getJSON("/dsp/get_structure", function (data) {
        cb(data);
    });
}

var last_container = undefined;

function add_control(html) {
    last_container.html(last_container.html() + html)
}

function create_dsp_controls(container) {
    last_container = container;
    load_dsp_structure(function (data) {
        data.modules.forEach(module => {
            module.groups.forEach(group => {
                group.controls.forEach(control => {
                    args = {
                        title: control.name,
                        ctrl_id: control.id
                    }
                    if (control.type == CTRL_VOLUME_SLEW) {
                        args.name = "volslew" + control.id;
                        generate_control("/static/tmpl/dsp_volslew.html", args, add_control);
                    }
                    if (control.type == CTRL_SECOND_ORDER_EQ) {
                        args.name = "soeq" + control.id;
                        generate_control("/static/tmpl/dsp_soeq.html", args, add_control);
                    }
                });
            })
        });
    });
}

var ws;

function dsp_register_websocket(websocket) {
    ws = websocket;
}

function update_volslew(ctrl_id, name) {
    var volume = $("#" + name + "_db_value").val();
    var slew = $("#" + name + "_slew_rate").val();

    $("#" + name + "_pill").text(volume + "dB");

    ws.send(JSON.stringify({
        "command": "dsp",
        "action": "update_volslew",
        "id": ctrl_id,
        "volume": volume,
        "slew": slew
    }));
}

function update_soeq(ctrl_id, name) {
    var boost = $("#" + name + "_db_value").val();
    var freq = $("#" + name + "_freq").val();

    $("#" + name + "_pill").text(freq + "Hz " + boost + "dB");

    ws.send(JSON.stringify({
        "command": "dsp",
        "action": "update_soeq",
        "id": ctrl_id,
        "filter_type": 0, // todo: input for this field
        "boost": boost,
        "freq": freq,
        "Q": 1.41, // todo: input for this field
        "state": 1 // todo: input for this field
    }))

}