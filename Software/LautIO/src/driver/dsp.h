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

#include "SigmaDSP.h"

#define CTRL_VOLSLEW 0
#define CTRL_SOEQ    1

void init_dsp();

void dsp_update_volslew(uint8_t id, float volume, uint8_t slew);
void dsp_update_soeq(uint8_t id, secondOrderEQ eq);