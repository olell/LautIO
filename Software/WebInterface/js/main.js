var lautio = null;
var device_addr = location.hostname;

var setup_process = 1;

function show_loading_screen() {
    $(".loading-overlay").removeClass("d-none");
}

function hide_loading_screen() {
    $(".loading-overlay").addClass("d-none");
}

function update_loading_message(msg) {
    $("#loading_msg").text(msg);
}

function connection_status_callback (state) {
    if (state) {
        $("#connection_state").text(`Connected`);
        if (setup_process == 1) { // continue with next setup step
            setup_process += 1;
            lautio.set_connection_status_update_interval(1000);
            setup_load_controls();
        }
    }
    else
        $("#connection_state").text("Not Connected!");
}

function setup_load_controls() {
    update_loading_message("Loading controls");
    lautio.update_dsp_controls();
}

function updated_controls_callback() {
    // todo
    if (setup_process == 2) {
        setup_process += 1;
        setup_load_status();
    }
}

function setup_load_status() {
    update_loading_message("Fetching amp status")
    lautio.update_amp_status();
}

function updated_amp_status_callback() {
    // todo
    if (setup_process == 3) {
        setup_process += 1;
        setup_done();
    }
}

function setup_done() {
    hide_loading_screen();
    show_overview_page();
}

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

function show_overview_page() {
    render_template("templates/overview.html", {
        "lautio": lautio,
    },
    function (html) {
        $("#main_container").html(html);
    })
}