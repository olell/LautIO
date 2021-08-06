var VIEW_OVERVIEW = 0;
var VIEW_CONTROLS = 1;
var VIEW_SETTINGS = 2;
var VIEW_STATUS = 3;
var current_view = VIEW_OVERVIEW;

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
        OverviewView.render();
    }
    else if (current_view == VIEW_CONTROLS) {
        $("#view_switch_controls").removeClass("btn-outline-secondary");
        $("#view_switch_controls").addClass("btn-secondary");
        ControlsView.render();
    }
    else if (current_view == VIEW_SETTINGS) {
        $("#view_switch_settings").removeClass("btn-outline-secondary");
        $("#view_switch_settings").addClass("btn-secondary");
        SettingsView.render();
    }
    else if (current_view == VIEW_STATUS) {
        $("#view_switch_status").removeClass("btn-outline-secondary");
        $("#view_switch_status").addClass("btn-secondary");
        StatusView.render();
    }
}

function update_view() {
    // Update current view
    if (current_view == VIEW_OVERVIEW) {
        OverviewView.update_values();
    }
    else if (current_view == VIEW_CONTROLS) {
        ControlsView.update_values();
    }
    else if (current_view == VIEW_SETTINGS) {
        SettingsView.update_values();
    }
    else if (current_view == VIEW_STATUS) {
        StatusView.update_values();
    }
}
