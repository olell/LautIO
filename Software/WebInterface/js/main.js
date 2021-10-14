var lautio = null;
var device_addr = location.hostname;

var setup_process = 1;

var console_visible = false;

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
            setup();
        }
    }
    else {
        $("#connection_state").text("Not Connected!");
        lautio.connect()
        console.log("Disconnected?")
    }
}

function setup() {
    if (setup_process == 1) {
        setup_process += 1;
        //lautio.set_connection_status_update_interval(10000);
        setup_load_controls();
    }
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
        setup_load_ui_config();
    }
}

function updated_ui_config_callback() {
    update_view();
    if (setup_process == 4) {
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
    update_loading_message("Fetching amp status");
    lautio.update_amp_status();
}

function setup_load_ui_config() {
    update_loading_message("Loading UI config");
    lautio.update_ui_config();
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

    lautio.ws_in_log_cb = console_log_wsin
    lautio.ws_out_log_cb = console_log_wsout
    lautio.set_connection_status_callback(connection_status_callback);
    lautio.set_connection_status_update_interval(2500);

    lautio.set_updated_controls_callback(updated_controls_callback);
    lautio.set_updated_amp_status_callback(updated_amp_status_callback);
    lautio.set_updated_ui_config_callback(updated_ui_config_callback);

    update_loading_message("Connecting to device");
    lautio.connect();

}