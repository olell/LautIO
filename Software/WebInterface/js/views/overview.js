class OverviewView {
    static render() {
        render_template("templates/overview.html", {
            "lautio": lautio,
        },
        function (html) {
            $("#main_container").html(html);
    
            // quick controls
            var quick_controls = lautio.dsp_controls.filter(obj => lautio.ui_config.quick_controls.includes(obj.id));
    
            quick_controls.filter(obj => obj.type == DSP_CONTROL_VOLSLEW).forEach(control => { // volslews
                VolslewControl.render(control, "#quick_controls", true, render_control);
            });
            quick_controls.filter(obj => obj.type == DSP_CONTROL_EQ_SECOND_ORDER).forEach(control => { // volslews
                SOEQControl.render(control, "#quick_controls", true, render_control);
            });
            
            OverviewView.update_values();
        })
    }

    static update_values() {
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
}

function amp_ab_en_toggler() {
    var new_stat = !lautio.amp_status.AB.reset_state;
    lautio.set_amp_reset_state(AMP_AB, new_stat);
}

function amp_cd_en_toggler() {
    var cur_stat = lautio.amp_status.CD.reset_state;
    lautio.set_amp_reset_state(AMP_CD, !cur_stat);
}

function amp_both_enable() {
    lautio.set_amp_reset_state(AMP_AB, 0);
    lautio.set_amp_reset_state(AMP_CD, 0);
}

function amp_both_disable() {
    lautio.set_amp_reset_state(AMP_AB, 1);
    lautio.set_amp_reset_state(AMP_CD, 1);
}