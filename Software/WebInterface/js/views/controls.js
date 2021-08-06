class ControlsView {
    static render() {
        render_template("templates/controls.html", {
            "lautio": lautio,
        },
        function (html) {
            $("#main_container").html(html);
    
            // all controls (todo currently all controls)
            // ditte is ja ooch irjenwie doppeldjemoppelt
            lautio.dsp_controls.filter(obj => obj.type == DSP_CONTROL_VOLSLEW).forEach(control => { // volslews
                VolslewControl.render(control, "#controls", true, render_control);
            });
            lautio.dsp_controls.filter(obj => obj.type == DSP_CONTROL_EQ_SECOND_ORDER).forEach(control => { // volslews
                SOEQControl.render(control, "#controls", true, render_control);
            });
            
            //update_overview_values();
        });
    }

    static update_values() {

    }
}