class SOEQControl {

    static render(control, container, append, renderer) {
        renderer(control, container, append, () => {});
    }

    static update_ui(control) {
        // update select
        $(`#ctrl_${control.id}_filter_type`).val(control.filter_type);

        // update checkboxes
        $(`#ctrl_${control.id}_state`).prop("checked", control.state);
        $(`#ctrl_${control.id}_phase`).prop("checked", control.phase);

        // update pill
        if (control.filter_type == SOEQ_FILTER_TYPE_PARAMETRIC) 
            $(`#ctrl_${control.id}_pill`).text(`Parametric filter (${control.freq}Hz ${control.boost}dB)`);
        if (control.filter_type == SOEQ_FILTER_TYPE_PEAKING) 
            $(`#ctrl_${control.id}_pill`).text(`Peaking filter (${control.freq}Hz ${control.boost}dB)`);
        if (control.filter_type == SOEQ_FILTER_TYPE_LOW_SHELF)
            $(`#ctrl_${control.id}_pill`).text(`Low Shelf filter (${control.freq}Hz)`);
        if (control.filter_type == SOEQ_FILTER_TYPE_HIGH_SHELF)
            $(`#ctrl_${control.id}_pill`).text(`High Shelf filter (${control.freq}Hz)`);
        if (control.filter_type == SOEQ_FILTER_TYPE_LOWPASS)
            $(`#ctrl_${control.id}_pill`).text(`Lowpass filter (${control.freq}Hz)`);
        if (control.filter_type == SOEQ_FILTER_TYPE_HIGHPASS)
            $(`#ctrl_${control.id}_pill`).text(`Highpass filter (${control.freq}Hz)`);
        if (control.filter_type == SOEQ_FILTER_TYPE_BUTTERWORTH_LOWPASS)
            $(`#ctrl_${control.id}_pill`).text(`Butterworth lowpass filter (${control.freq}Hz ${control.gain}dB)`);
        if (control.filter_type == SOEQ_FILTER_TYPE_BUTTERWORTH_HIGHPASS)
            $(`#ctrl_${control.id}_pill`).text(`Butterworht highpass filter (${control.freq}Hz ${control.gain}dB)`);
        if (control.filter_type == SOEQ_FILTER_TYPE_BESSEL_LOWPASS)
            $(`#ctrl_${control.id}_pill`).text(`Bessel lowpass filter (${control.freq}Hz ${control.gain}dB)`);
        if (control.filter_type == SOEQ_FILTER_TYPE_BESSEL_HIGHPASS)
            $(`#ctrl_${control.id}_pill`).text(`Bessel highpass filter (${control.freq}Hz ${control.gain}dB)`);


        // update UI
        if (control.filter_type == SOEQ_FILTER_TYPE_PARAMETRIC ||
            control.filter_type == SOEQ_FILTER_TYPE_PEAKING) { // Peaking filter

            // Type & frequency & active & phase always visible
            $(`#ctrl_${control.id}_boost_container`).show();
            $(`#ctrl_${control.id}_gain_container`).hide();
            $(`#ctrl_${control.id}_q_container`).show();
            $(`#ctrl_${control.id}_s_container`).hide();
            $(`#ctrl_${control.id}_bandwidth_container`).hide();

        }
        else if (control.filter_type == SOEQ_FILTER_TYPE_LOW_SHELF ||
                control.filter_type == SOEQ_FILTER_TYPE_HIGH_SHELF) {
            $(`#ctrl_${control.id}_boost_container`).show();
            $(`#ctrl_${control.id}_gain_container`).show();
            $(`#ctrl_${control.id}_q_container`).hide();
            $(`#ctrl_${control.id}_s_container`).show();
            $(`#ctrl_${control.id}_bandwidth_container`).hide();
        }
        else if (control.filter_type == SOEQ_FILTER_TYPE_LOWPASS ||
                control.filter_type == SOEQ_FILTER_TYPE_HIGHPASS) {

            $(`#ctrl_${control.id}_gain_container`).hide();
            $(`#ctrl_${control.id}_boost_container`).hide();
            $(`#ctrl_${control.id}_q_container`).show();
            $(`#ctrl_${control.id}_s_container`).hide();
            $(`#ctrl_${control.id}_bandwidth_container`).hide();

        }
        else if (control.filter_type == SOEQ_FILTER_TYPE_BANDPASS ||
                control.filter_type == SOEQ_FILTER_TYPE_BANDSTOP) {

            $(`#ctrl_${control.id}_gain_container`).show();
            $(`#ctrl_${control.id}_boost_container`).hide();
            $(`#ctrl_${control.id}_q_container`).hide();
            $(`#ctrl_${control.id}_s_container`).hide();
            $(`#ctrl_${control.id}_bandwidth_container`).show();

        }
        else if (control.filter_type == SOEQ_FILTER_TYPE_BUTTERWORTH_HIGHPASS ||
                control.filter_type == SOEQ_FILTER_TYPE_BUTTERWORTH_HIGHPASS) {

            $(`#ctrl_${control.id}_gain_container`).show();
            $(`#ctrl_${control.id}_boost_container`).hide();
            $(`#ctrl_${control.id}_q_container`).hide();
            $(`#ctrl_${control.id}_s_container`).hide();
            $(`#ctrl_${control.id}_bandwidth_container`).hide();
        }
        else if (control.filter_type == SOEQ_FILTER_TYPE_BESSEL_LOWPASS ||
                control.filter_type == SOEQ_FILTER_TYPE_BESSEL_HIGHPASS) {

            $(`#ctrl_${control.id}_gain_container`).show();
            $(`#ctrl_${control.id}_boost_container`).hide();
            $(`#ctrl_${control.id}_q_container`).hide();
            $(`#ctrl_${control.id}_s_container`).hide();
            $(`#ctrl_${control.id}_bandwidth_container`).hide();
        }
    }

    static update(control, lautio) {
        // load new values from inputs
        var new_filter_type = parseInt($(`#ctrl_${control.id}_filter_type`).val());
        var new_q = parseFloat($(`#ctrl_${control.id}_q`).val());
        var new_s = parseFloat($(`#ctrl_${control.id}_s`).val());
        var new_freq = parseFloat($(`#ctrl_${control.id}_freq`).val());
        var new_bandwidth = parseFloat($(`#ctrl_${control.id}_bandwidth`).val());
        var new_gain = parseFloat($(`#ctrl_${control.id}_gain`).val());
        var new_boost = parseFloat($(`#ctrl_${control.id}_boost`).val());
        var new_state = $(`#ctrl_${control.id}_state`).prop("checked") ? 1 : 0;
        var new_phase = $(`#ctrl_${control.id}_phase`).prop("checked") ? 1 : 0;
    
        control.filter_type = new_filter_type;
        control.Q = new_q;
        control.S = new_s;
        control.freq = new_freq;
        control.bandwidth = new_bandwidth;
        control.gain = new_gain;
        control.boost = new_boost;
        control.state = new_state;
        control.phase = new_phase;
        SOEQControl.update_ui(control);
        lautio.update_control(control);
    }

}