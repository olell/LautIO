var lautio = null;
var device_addr = location.hostname;

var setup_process = 1;

var VIEW_OVERVIEW = 0;
var VIEW_CONTROLS = 1;
var VIEW_SETTINGS = 2;
var VIEW_STATUS = 3;
var current_view = VIEW_OVERVIEW;

/* Loading Screen */
function show_loading_screen() {
    $(".loading-overlay").removeClass("d-none");
}

function hide_loading_screen() {
    $(".loading-overlay").addClass("d-none");
}

function update_loading_message(msg) {
    $("#loading_msg").text(msg);
}

/* Callbacks */
function connection_status_callback (state) {
    if (state) {
        $("#connection_state").text(`Connected`);
        if (setup_process == 1) { // continue with next setup step
            setup_process += 1;
            lautio.set_connection_status_update_interval(10000);
            setup_load_controls();
        }
    }
    else
        $("#connection_state").text("Not Connected!");
}


function updated_controls_callback() {
    // todo
    if (setup_process == 2) {
        setup_process += 1;
        setup_load_status();
    }
}


function updated_amp_status_callback() {
    update_view();
    if (setup_process == 3) {
        setup_process += 1;
        setup_done();
    }
}

/* Setup functions */
function setup_load_controls() {
    update_loading_message("Loading controls");
    lautio.update_dsp_controls();
}

function setup_load_status() {
    update_loading_message("Fetching amp status")
    lautio.update_amp_status();
}

function setup_done() {
    hide_loading_screen();
    current_view = VIEW_OVERVIEW;
    render_view();
}

// Startup function
function startup () {
    
    show_loading_screen();
    update_loading_message("Initialising");

    lautio = new LautIO(device_addr);
    lautio.set_connection_status_callback(connection_status_callback);
    lautio.set_connection_status_update_interval(1500);

    lautio.set_updated_controls_callback(updated_controls_callback);
    lautio.set_updated_amp_status_callback(updated_amp_status_callback);

    update_loading_message("Connecting to device");
    lautio.connect();

}

// Views
function change_view(view) {
    current_view = view;
    render_view();
}

function render_view() {
    // Render current view
    $("#view_switch_overview").addClass("btn-outline-secondary");
    $("#view_switch_overview").removeClass("btn-secondary");
    $("#view_switch_controls").addClass("btn-outline-secondary");
    $("#view_switch_controls").removeClass("btn-secondary");
    $("#view_switch_settings").addClass("btn-outline-secondary");
    $("#view_switch_settings").removeClass("btn-secondary");
      $("#view_switch_status").addClass("btn-outline-secondary");
      $("#view_switch_status").removeClass("btn-secondary");
    
    if (current_view == VIEW_OVERVIEW) {
        $("#view_switch_overview").removeClass("btn-outline-secondary");
        $("#view_switch_overview").addClass("btn-secondary");
        render_overview_view();
    }
    else if (current_view == VIEW_CONTROLS) {
        $("#view_switch_controls").removeClass("btn-outline-secondary");
        $("#view_switch_controls").addClass("btn-secondary");
        render_controls_view();
    }
    else if (current_view == VIEW_SETTINGS) {
        $("#view_switch_settings").removeClass("btn-outline-secondary");
        $("#view_switch_settings").addClass("btn-secondary");
        render_settings_view();
    }
    else if (current_view == VIEW_STATUS) {
        $("#view_switch_status").removeClass("btn-outline-secondary");
        $("#view_switch_status").addClass("btn-secondary");
        render_status_view();
    }
}

function update_view() {
    // Update current view
    if (current_view == VIEW_OVERVIEW) {
        update_overview_values();
    }
    else if (current_view == VIEW_CONTROLS) {
    }
    else if (current_view == VIEW_SETTINGS) {
    }
    else if (current_view == VIEW_STATUS) {
    }
}

// Overview 
function render_overview_view() {
    render_template("templates/overview.html", {
        "lautio": lautio,
    },
    function (html) {
        $("#main_container").html(html);

        // quick controls (todo currently all controls)
        lautio.dsp_controls.filter(obj => obj.type == DSP_CONTROL_VOLSLEW).forEach(control => { // volslews
            render_volslew(control, "#quick_controls", true);
        });
        lautio.dsp_controls.filter(obj => obj.type == DSP_CONTROL_EQ_SECOND_ORDER).forEach(control => { // volslews
            render_soeq(control, "#quick_controls", true);
        });
        
        update_overview_values();
    })
}

function update_overview_values() {
    // Amp AB status values
    if (lautio.amp_status.AB.reset_state) {
        $("#amp_ab_enable_button").text("Enable");
        $("#amp_ab_enable_button").addClass("btn-outline-success");
        $("#amp_ab_enable_button").removeClass("btn-outline-danger");
        $("#amp_ab_state").text("in reset state");
    }
    else {
        $("#amp_ab_enable_button").text("Disable");
        $("#amp_ab_enable_button").removeClass("btn-outline-success");
        $("#amp_ab_enable_button").addClass("btn-outline-danger");
        $("#amp_ab_state").text("active");
    }

    if (lautio.amp_status.AB.otw_state)
        $("#amp_ab_otw_clip_state").text("Yes");
    else
        $("#amp_ab_otw_clip_state").text("No");

    if (lautio.amp_status.AB.fault_state)
        $("#amp_ab_fault_state").text("Yes");
    else
        $("#amp_ab_fault_state").text("No");

    // Amp CD status values
    if (lautio.amp_status.CD.reset_state) {
        $("#amp_cd_enable_button").text("Enable");
        $("#amp_cd_enable_button").addClass("btn-outline-success");
        $("#amp_cd_enable_button").removeClass("btn-outline-danger");
        $("#amp_cd_state").text("in reset state");
    }
    else {
        $("#amp_cd_enable_button").text("Disable");
        $("#amp_cd_enable_button").removeClass("btn-outline-success");
        $("#amp_cd_enable_button").addClass("btn-outline-danger");
        $("#amp_cd_state").text("active");
    }

    if (lautio.amp_status.CD.otw_state)
        $("#amp_cd_otw_clip_state").text("Yes");
    else
        $("#amp_cd_otw_clip_state").text("No");

    if (lautio.amp_status.CD.fault_state)
        $("#amp_cd_fault_state").text("Yes");
    else
        $("#amp_cd_fault_state").text("No");


}
// Inputs
function amp_ab_en_toggler() {
    var new_stat = !lautio.amp_status.AB.reset_state;
    lautio.set_amp_reset_state(AMP_AB, new_stat);
}

function amp_cd_en_toggler() {
    var cur_stat = lautio.amp_status.CD.reset_state;
    lautio.set_amp_reset_state(AMP_CD, !cur_stat);
}

// controls
function render_controls_view() {
    $("#main_container").html("controls lol");
}
// settings
function render_settings_view() {
    $("#main_container").html("settings lol");
}
// status
function render_status_view() {
    $("#main_container").html("status lol");
}


function update_control(control_id) {
    var control = lautio.dsp_controls.filter(obj => obj.id == control_id)[0];

    if (control.type == DSP_CONTROL_VOLSLEW) {
        update_volslew(control);
    }
    else if (control.type == DSP_CONTROL_EQ_SECOND_ORDER) {
        update_soeq(control);
    }
}

function render_volslew(control, container, append) {
    render_template("templates/controls/volslew.html", {"lautio": lautio, "control": control}, function(html) {
        // put html to container
        if (append)
            $(container).html($(container).html() + html);
        else
            $(container).html(html);
    })
}

function update_volslew(control) {
    var new_db_val = $(`#${control.name}_db_value`).val();
    var new_slew_val = $(`#${control.name}_slew_rate`).val();
    control.volume = new_db_val;
    control.slew = new_slew_val;
    lautio.update_control(control);
}

function render_soeq(control, container, append) {
    render_template("templates/controls/2nd_order_eq.html", {"lautio": lautio, "control": control}, function(html) {
        // put html to container
        if (append)
            $(container).html($(container).html() + html);
        else
            $(container).html(html);

        update_soeq_ui(control);
        
    })
}

function update_soeq_ui(control) {
    // update select
    $(`#${control.name}_filter_type`).val(control.filter_type);

    // update checkboxes
    $(`#${control.name}_state`).prop("checked", control.state);
    $(`#${control.name}_phase`).prop("checked", control.phase);

    // update UI
    if (control.filter_type == 0) { // Peaking filter
        $("#" + control.name + "_pill").text(`Peaking filter (${control.freq}Hz ${control.boost}dB)`);
        gain = 0;
        s = 0;
        bandwidth = 0;

        // Type & frequency & active & phase always visible
        $(`#${control.name}_boost_container`).show();
        $(`#${control.name}_gain_container`).hide();
        $(`#${control.name}_q_container`).show();
        $(`#${control.name}_s_container`).hide();
        $(`#${control.name}_bandwidth_container`).hide();

    }
    else if (control.filter_type == 4) { // Low Pass
        $(`#${control.name}_pill`).text(`Low Pass filter (${control.freq}Hz)`)
        gain = 0;
        boost = 0;
        s = 0;
        bandwidth = 0;

        $(`#${control.name}_gain_container`).hide();
        $(`#${control.name}_boost_container`).hide();
        $(`#${control.name}_q_container`).show();
        $(`#${control.name}_s_container`).hide();
        $(`#${control.name}_bandwidth_container`).hide();

    }
    else if (control.filter_type == 5) { // High Pass
        $(`#${control.name}_pill`).text(`High Pass filter (${control.freq}Hz)`)
        gain = 0;
        boost = 0;
        s = 0;
        bandwidth = 0;

        $(`#${control.name}_gain_container`).hide();
        $(`#${control.name}_boost_container`).hide();
        $(`#${control.name}_q_container`).show();
        $(`#${control.name}_s_container`).hide();
        $(`#${control.name}_bandwidth_container`).hide();

    }
    else if (control.filter_type == 6) { // Band Pass
        $(`#${control.name}_pill`).text(`Band Pass filter (${control.freq}Hz ${control.gain}dB)`)
        boost = 0;
        q = 0;
        s = 0;

        $(`#${control.name}_gain_container`).show();
        $(`#${control.name}_boost_container`).hide();
        $(`#${control.name}_q_container`).hide();
        $(`#${control.name}_s_container`).hide();
        $(`#${control.name}_bandwidth_container`).show();

    }

}

function update_soeq(control) {
    // load new values from inputs
    var new_filter_type = parseInt($(`#${control.name}_filter_type`).val());
    var new_q = parseFloat($(`#${control.name}_q`).val());
    var new_s = parseFloat($(`#${control.name}_s`).val());
    var new_freq = parseFloat($(`#${control.name}_freq`).val());
    var new_bandwidth = parseFloat($(`#${control.name}_bandwidth`).val());
    var new_gain = parseFloat($(`#${control.name}_gain`).val());
    var new_boost = parseFloat($(`#${control.name}_boost`).val());
    var new_state = $(`#${control.name}_state`).prop("checked") ? 1 : 0;
    var new_phase = $(`#${control.name}_phase`).prop("checked") ? 1 : 0;

    control.filter_type = new_filter_type;
    control.Q = new_q;
    control.S = new_s;
    control.freq = new_freq;
    control.bandwidth = new_bandwidth;
    control.gain = new_gain;
    control.boost = new_boost;
    control.state = new_state;
    control.phase = new_phase;
    
    update_soeq_ui(control);
    lautio.update_control(control);
}