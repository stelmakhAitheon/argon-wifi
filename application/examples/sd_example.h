#pragma once

#include "Common.h"
#include "Logger.h"

#include "SDBlockDevice.h"

#define SD_MOSI                         P0_20
#define SD_MISO                         P0_21
#define SD_CLK                          P0_19
#define SD_CS                           P0_17

void sd_example() {
    Logger::getInstance()->addMessage("SD_EXAMPLE \r\n");

    // SDBlockDevice sd(SD_MOSI, SD_MISO, SD_CLK, SD_CS);
    // int init = sd.init();
    // Logger::getInstance()->addMessage("SD_EXAMPLE Init = %d \r\n", init);
    SPI spi(SD_MOSI, SD_MISO, SD_CLK); // mosi, miso, sclk
    DigitalOut cs(SD_CS);
    
    cs = 1;

    spi.format(8, 3);
    spi.frequency(1000000);
    cs = 0;

    spi.write(0x8F);
    int whoami = spi.write(0x10);
    printf("WHOAMI register = 0x%X\n", whoami);


    Logger::getInstance()->addMessage("SD_EXAMPLE FINISH \r\n");
}