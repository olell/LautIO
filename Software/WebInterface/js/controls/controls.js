function update_control_settings(control_id) {
    var control = lautio.dsp_controls.filter(obj => obj.id == control_id)[0];

    var new_name = $(`#ctrl_${control.id}_name`).val();
    var new_ro = $(`#ctrl_${control.id}_ro`).prop("checked") ? 1 : 0;
    var new_change = $(`#ctrl_${control.id}_change`).prop("checked") ? 1 : 0;

    control.name = new_name;
    control.ro = new_ro;
    control.change = new_change;

    console.log(control.name);
    
    update_control(control.id);
}

function render_control(control, container, append, cb) {
    var control_name;
    if (control.type == DSP_CONTROL_VOLSLEW) control_name = "volslew";
    if (control.type == DSP_CONTROL_EQ_SECOND_ORDER) control_name = "2nd_order_eq";
    render_template(`templates/controls/${control_name}.html`, {"lautio": lautio, "control": control}, function(ctrl_html) {
        render_template("templates/controls/settings.html", {"lautio": lautio, "control": control}, function(settings_html) {
            render_template("templates/controls/control.html", {"control": control, "lautio": lautio, "body": ctrl_html, "modal": settings_html}, function(html) {
                // put html to container
                if (append)
                    $(container).html($(container).html() + html);
                else
                    $(container).html(html);
                
                update_control_ui(control);
                cb();
            });  
        })
    })
}

function update_control_ui(control) {
    // update control common settings
    $(`#ctrl_${control.id}_name`).val(control.name);
    $(`#ctrl_${control.id}_ro`).prop("checked", control.ro);
    $(`#ctrl_${control.id}_change`).prop("checked", control.change);

    $(`#ctrl_${control.id}_name_header`).text(control.name);
    $(`#ctrl_${control.id}_fieldset`).prop("disabled", control.ro);

    if (control.type == DSP_CONTROL_VOLSLEW) {
        VolslewControl.update_ui(control);
    }
    else if (control.type == DSP_CONTROL_EQ_SECOND_ORDER) {
        SOEQControl.update_ui(control);
    }
}

function update_control(control_id) {
    var control = lautio.dsp_controls.filter(obj => obj.id == control_id)[0];

    if (control.type == DSP_CONTROL_VOLSLEW) {
        VolslewControl.update(control, lautio);
    }
    else if (control.type == DSP_CONTROL_EQ_SECOND_ORDER) {
        SOEQControl.update(control, lautio);
    }

    update_control_ui(control);
}