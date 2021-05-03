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
    }
    else if (amp == AMP_CD) {
        log_debug("Putting Ch. C + D Amp in reset state %d", state);
        digitalWrite(AMP_CD_RESET, 1 - state);
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