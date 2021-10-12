class SettingsView {
    static render() {
        render_template("templates/settings.html", {
            "lautio": lautio,
        },
        function (html) {
            $("#main_container").html(html);
        });
    }

    static update_values() {

    }
}

function reboot() {
    lautio.reboot();
}