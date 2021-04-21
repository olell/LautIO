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

// Amp AB Controls
#define AMP_AB_MUTE_PIN    17 // IO17
#define AMP_AB_STANDBY_PIN 16 // IO16

// Amp CD Controls
#define AMP_CD_MUTE_PIN    33 // IO33
#define AMP_CD_STANDBY_PIN 32 // IO32

// DSP Controls
#define DSP_RESET_PIN      14 // IO14

// I2S Output
#define DSP_IN_DATA_PIN    26 // IO26
#define DSP_IN_BCLK_PIN    27 // IO27
#define DSP_IN_LRCLK_PIN   13 // IO13

//#define HARDWARE_HAS_SDCARD // Uncomment on hardware versions which have sd card slots

#define HARDWARE_NAME "proto v1"