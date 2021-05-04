/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#include "stdint.h"
#include "Arduino.h"

#include "sys_config.h"

#include "util/log.h"
#include "driver/amp.h"

bool amp_ab_reset_state = 0;
bool amp_cd_reset_state = 0;

void init_amps() {
    /*
    This method initialises all amplifier pins and puts all amps in reset state.
    */

    log_info("Initing Amplifiers");

    // Pin Modes
    pinMode(AMP_AB_OTW, INPUT);
    pinMode(AMP_AB_FAULT, INPUT);
    pinMode(AMP_AB_RESET, OUTPUT);

    pinMode(AMP_CD_OTW, INPUT);
    pinMode(AMP_CD_FAULT, INPUT);
    pinMode(AMP_CD_RESET, OUTPUT);

    log_debug("Amp AB OTW:   %s", get_amp_otw_state(AMP_AB) ? "Yes" : "No");
    log_debug("Amp AB Fault: %s", get_amp_fault_state(AMP_AB) ? "Yes" : "No");
    log_debug("Amp CD OTW:   %s", get_amp_otw_state(AMP_CD) ? "Yes" : "No");
    log_debug("Amp CD Fault: %s", get_amp_fault_state(AMP_CD) ? "Yes" : "No");

    // Put both amps in reset state
    amp_set_reset_state(AMP_AB, 1);
    amp_set_reset_state(AMP_CD, 1);


}

void amp_set_reset_state(uint8_t amp, uint8_t state) {
    /*
    This method puts an amp in the given reset state:
    state 1: reset state
    state 0: running
    */
    if (amp == AMP_AB) {
        log_debug("Putting Ch. A + B Amp in reset state %d", state);
        digitalWrite(AMP_AB_RESET, 1 - state);
        amp_ab_reset_state = state;
    }
    else if (amp == AMP_CD) {
        log_debug("Putting Ch. C + D Amp in reset state %d", state);
        digitalWrite(AMP_CD_RESET, 1 - state);
        amp_cd_reset_state = state;
    }

}

void reset_amp(uint8_t amp) {
    /*
    This method pulses the reset state (e.g. to reset after a fault condition is solved)
    */
    amp_set_reset_state(amp, 1);
    delay(50);
    amp_set_reset_state(amp, 0);
}

bool get_amp_reset_state(uint8_t amp) {
    // Get reset state of amp
    if (amp == AMP_AB) return amp_ab_reset_state;
    else return amp_cd_reset_state;
}

bool get_amp_otw_state(uint8_t amp) {
    // Get Clipping / Over Temp Warning state
    if (amp == AMP_AB) {
        return digitalRead(AMP_AB_OTW) == 0;
    }
    else if (amp == AMP_CD) {
        return digitalRead(AMP_CD_OTW) == 0;
    }
}

bool get_amp_fault_state(uint8_t amp) {
    // Get Fault state
    if (amp == AMP_AB) {
        return digitalRead(AMP_AB_FAULT) == 0;
    }
    else {
        return digitalRead(AMP_CD_FAULT) == 0;
    }
}