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

// control type defs
#define DSP_CONTROL_VOLSLEW          0
#define DSP_CONTROL_MUX              1
#define DSP_CONTROL_EQ_SECOND_ORDER  2
#define DSP_CONTROL_DC_SOURCE        3
#define DSP_CONTROL_SINE_SOURCE      4
#define DSP_CONTROL_SQUARE_SOURCE    5
#define DSP_CONTROL_SAWTOOTH_SOURCE  6
#define DSP_CONTROL_TRIANGLE_SOURCE  7
#define DSP_CONTROL_AUDIO_DELAY      8
#define DSP_CONTROL_EQ_FIRST_ORDER   9
#define DSP_CONTROL_GAIN            10
#define DSP_CONTROL_DEMUX           11
#define DSP_CONTROL_SOFT_CLIP       12
#define DSP_CONTROL_HARD_CLIP       13
#define DSP_CONTROL_COMPRESSOR_RMS  14
#define DSP_CONTROL_COMPRESSOR_PEAK 15
#define DSP_CONTROL_TONE_CONTROL    16
#define DSP_CONTROL_STATE_VARIABLE  17
#define DSP_CONTROL_MUTE_ADC        18
#define DSP_CONTROL_MUTE_DAC        19

#define PROGRAM_REGSIZE 5
#define PARAM_REGSIZE 4

void init_dsp();
bool dsp_ping();

void dsp_change_firmware(const char* firmware_name);
DynamicJsonDocument* get_controls_json();

void dsp_update_from_updated_json(JsonObject control);