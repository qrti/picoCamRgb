// cam_wiz rgb V0.8 220330 qrt@qland.de

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "keys.h"
#include "text.h"

#define OLED_SPI_SCK_PIN    2                           // clock
#define OLED_SPI_TX_PIN     3                           // send
#define OLED_SPI_DC_PIN     4                           // data/command
#define OLED_SPI_CS_PIN     5                           // chip select
#define OLED_RST_PIN        20                          // reset

#define OLED_CMD            0                           // command
#define OLED_DATA           1                           // data

#define OLED_XSIZE          128                         // x size
#define OLED_YSIZE          128                         // y
#define VIDSIZE             (OLED_XSIZE * OLED_YSIZE)   // video buffer word size

#define OPTCOL_BACK         __bswap16(0b0000100000100001)
#define OPTCOL_TEXT         __bswap16(0b0000000000011100)
#define OPTSTART_Y          110

// #define SPI_INSTANCE        1
// #define spi_inst            (__CONCAT(spi, SPI_INSTANCE)) 

#define spi_inst            spi_default                 // default instance 0
#define OLED_SPIFREQ        30000000                    // spi freqeuncy

const uint8_t initSeq[] = {     // init sequence
    1, 0xfd, 0x12,              // unlock driver MCU
    1, 0xfd, 0xb1,              // command a2, b1, b3, bb, be, c1 accessible if in unlock state  
    0, 0xae,                    // sleep mode on, display off
    0, 0xa4,                    // all off
    // 2, 0x15, 0x00, 0x7f,        // set column address, start 00, end 127    
    // 2, 0x75, 0x00, 0x7f,        // set row address,    start 00, end 127   
    // 1, 0xb3, 0xf1,              // front clock divider, oscillator frequency div
    // 1, 0xca, 0x7f,              // mux ratio

    1, 0xa0, 0x34,              // 65k color, color sequence CBA (BRG)
    // 1, 0xa0, 0x30,              // 65k color, color sequence ABC (RGB)

    // 1, 0xa1, 0x00,              // set display start line, start 00 line
    1, 0xa2, 0x00,              // set display offset
    1, 0xab, 0x01,              // enable Vdd regulator
    // 3, 0xb4, 0xa0, 0xb5, 0x55,  // sement low voltage
    // 3, 0xc1, 0xc8, 0x80, 0xc0,  // contrast ABC
    // 1, 0xc7, 0x0f,              // master contrast
    1, 0xb1, 0x32,              // reset precharge
    3, 0xb2, 0xa4, 0x00, 0x00,  // display enhancement
    1, 0xbb, 0x17,              // precharge voltage
    1, 0xb6, 0x01,              // second precharge period
    1, 0xbe, 0x05,              // voltage
    0, 0xa6,                    // display mode normal
    255                         // end of sequence
};

class Oled
{
    public:
        static void init();
        static void showRainbow();
        static void display();
        static void clrLine(uint8_t ys, uint8_t h);
        static void fillRight(uint8_t xs, uint8_t ys, uint8_t h);

        static uint16_t col565(float x, float fy);
        static uint16_t noc(float x, float fy, uint8_t maxc, uint8_t shift);
        static void set(uint8_t x, uint8_t y, uint16_t color);
        static void clear();
        static void show();

    private:
        static void reset();
        static void spiInit();
        static void spiWriteCmd(uint8_t reg, uint8_t* buf, size_t len);
        static void spiWriteData(uint8_t* buf, size_t len);

        static uint16_t* vidBuf;
        static uint8_t oledX, oledY;
};
