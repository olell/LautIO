class VolslewControl {
    static render(control, container, append, renderer) {
        renderer(control, container, append, ()=>{});
    }

    static update_ui(control) {

    }

    static update(control, lautio) {
        var new_db_val = $(`#ctrl_${control.id}_db_value`).val();
        var new_slew_val = $(`#ctrl_${control.id}_slew_rate`).val();
        control.volume = new_db_val;
        control.slew = new_slew_val;
        lautio.update_control(control);
    }
}