/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

function debug_console_keydown(evt) {
    evt = evt || window.event;
    if(evt.keyCode == 13) {
        var payload = $("#debug_console_input").val();
        console_log_wsout(payload);
        if (lautio !== null) {
            if (lautio.connected) {
                lautio.websocket.send(payload);
            }
        }
        $("#debug_console_input").val("");
    }

}

$(window).keydown(function(event) {
    if(event.key == '#') {
        console_visible = !console_visible;
        if (console_visible) {
            $("#debug_console").removeClass("d-none");
        } else {
            $("#debug_console").addClass("d-none");
        }
    }
});

function debug_console_add(text) {
    var cval = $("#debug_console_output").text();
    $("#debug_console_output").text(cval + "\n" + text);
    $('#debug_console_output').scrollTop($('#debug_console_output')[0].scrollHeight);
}

function console_log_wsout(payload) {
    debug_console_add("---> " + payload);
}

function console_log_wsin(payload) {
    debug_console_add("<--- " + payload);
}