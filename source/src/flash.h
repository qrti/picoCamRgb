// cam_wiz rgb V0.8 220330 qrt@qland.de

#pragma once

#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "pico/multicore.h"

#define FLASH_TARGET_OFFSET (256 * 1024)

class Flash
{
    public:
        static void init(uint8_t size);
        static bool isValid() { return valid; }
        static void write(uint8_t ad, uint8_t b) { flashBuf[ad] = b; }
        static uint8_t read(uint8_t ad) { return flashBuf[ad]; }
        static void flash();
        static void addFletcher();
        static bool checkFletcher();
        static uint8_t getNextValue(uint8_t def);
        static void setValue(uint8_t ad, uint8_t v);

    private:        
        static uint8_t* flashBuf;
        static uint8_t size;
        static uint8_t index;
        static bool valid;
};
