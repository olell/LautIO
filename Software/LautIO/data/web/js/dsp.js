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
            </button>
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
        <input type="range" class="form-range" id="` + name + `_db_value" min="-50" max="0" value="0" step="0.5">
        <span class="font-weight-bold ms-2">0dB</span>
    </div>
    <input type="hidden" id="` + name + `_slew_rate">
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