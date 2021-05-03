/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#pragma once

#include "sys_config.h"

#define AMP_AB 0
#define AMP_CD 1

void init_amps();
void amp_set_reset_state(uint8_t amp, uint8_t state);
void reset_amp(uint8_t amp);