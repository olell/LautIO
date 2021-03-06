/*
 *
 * This file is part of LautIO
 *
 * Hardware configuration file for the second prototype board.
 * Containing pin definitions and feature switches
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#pragma once

// Amp AB Controls
#define AMP_AB_OTW         5
#define AMP_AB_FAULT       18
#define AMP_AB_RESET       26

// Amp CD Controls
#define AMP_CD_OTW         19
#define AMP_CD_FAULT       23
#define AMP_CD_RESET       27

// Battery
#define VBAT_MEASURE       35

// DSP Controls
#define DSP_RESET_PIN      25
#define DSP_I2C_ADDRESS    0x34 //(0x68 >> 1) & 0xFE (7-Bit addr)

// LED
#define SYS_LED_PIN        17

// Microcontroller is not wired via i2s to dsp
#define HARDWARE_NO_I2S

// The board contains a micro sdcard socket
#define HARDWARE_HAS_SDCARD

#define HARDWARE_NAME "proto v2"