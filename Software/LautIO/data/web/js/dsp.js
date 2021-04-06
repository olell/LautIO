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

function generate_container(name, title, controls) {
    return `
    <div class="card">
        <h5 class="card-header">` + title + `
            <button class="btn btn-primary float-end" data-bs-toggle="collapse" data-bs-target="#` + name + `Collapse" aria-controls="` + name + `Collapse" aria-expanded="true">
                Show/Hide
            </button><br class="mb-1">

            <span class="badge rounded-pill bg-secondary" id="` + name + `_pill">N/A</span>
        </h5>
        <div class="collapse show" id="` + name + `Collapse">
            <div class="card-body">
                ` + controls + `
            </div>
        </div>
    </div>`;
} 

function generate_volume_control(name, title, ctrl_id) {
    return generate_container(name, title, `
    <label for="` + name + `db_value_range" class="form-label">dB Value</label>
    <div class="d-flex justify-content-center">
        <span class="font-weight-bold me-2">-50dB</span>
        <input type="range" oninput="update_volslew(` + ctrl_id + `, '` + name + `')" class="form-range" id="` + name + `_db_value" min="-50" max="0" value="0" step="0.5">
        <span class="font-weight-bold ms-2">0dB</span>
    </div>
    <div class="mt-3">
        <label for="` + name + `_slew_rate">Slew Rate
        <input type="number" class="form-control" id="` + name + `_slew_rate" min="1" max="15" value="12">
        </label>
    </div>
    `);
}

function load_dsp_structure(cb) {
    $.getJSON("/dsp/get_structure", function (data) {
        cb(data);
    });
}

function create_dsp_controls(container) {
    load_dsp_structure(function (data) {
        var modules = [];
        data.modules.forEach(module => {
            var groups = [];
            module.groups.forEach(group => {
                var controls = [];
                group.controls.forEach(control => {
                    if (control.type == CTRL_VOLUME_SLEW) {
                        controls.push(generate_volume_control("volslew" + control.id, control.name, control.id));
                    }
                });
                groups.push(controls.join("\n")); // just a test placeholder
            })
            modules.push(groups.join("\n")); // just a test placeholder
        });

        console.log(modules);
        container.html(modules);
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