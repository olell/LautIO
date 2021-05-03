/*
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
 */

#include "sys_config.h"

#include "util/log.h"
#include "driver/filesystem.h"

#if FILESYSTEM_TYPE == FS_SPIFFS /* Place SPIFSS related code in this block */

#include <SPIFFS.h>
void init_filesystem() {
    /*
    This method initialises the SPIFFS filesystem
    */
    log_info("Initialising SPIFFS filesystem");
    if(!SPIFFS.begin(true)) {
        log_fatal("An fatal error occured while mounting SPIFFS. Please reset controller!");
        while(true) {}; // Halt; todo: automatic reboot?
    }
    log_info("SPIFFS up and running!");
}

#endif

#if FILESYSTEM_TYPE == FS_SDCARD /* Place SD Card related code in this block */
#include <SD_MMC.h>

void init_filesystem() {
    /*
    This method initialises the SD/MMC card and logs some information about it
    */
    log_info("Initialising SD Card filesystem");
    if(!SD_MMC.begin("/sdcard", true)) { // using 1 bit mode
        log_fatal("An fatal error occured while mounting SD Card. Please reset controller!");
        while(true) {}; // Halt; todo: some restarting?
    }

    // Detected if no card is inserted
    uint8_t card_type = SD_MMC.cardType();
    String card_name = "";
    if (card_type == CARD_NONE) {
        log_fatal("No SD Card inserted! Please insert a card and restart the controller!");
        while(true) {};
    }

    // Log card type
    else if (card_type == CARD_MMC)  card_name = "MMC";
    else if (card_type == CARD_SD)   card_name = "SDSC";
    else if (card_type == CARD_SDHC) card_name = "SDHC";
    else card_name = "unknown";
    log_debug("Inserted card type is %s", card_name.c_str());

    // Log card size and left space on card
    uint64_t card_size = SD_MMC.cardSize() / 0x100000;
    log_debug("Card size is %lluMB", card_size);
    uint64_t card_left = (SD_MMC.totalBytes() - SD_MMC.usedBytes()) / 0x100000;
    log_debug("Card space left: %lluMB", card_left);

    log_info("SD Card up and running!");

}
#endif