// cam_wiz rgb V0.8 220330 qrt@qland.de

#pragma once

#include "pico/stdlib.h"
#include "option.h"
#include "flash.h"

#define OPT_PIN             26
#define PLUS_PIN            27
#define MINUS_PIN           28
#define KEY_MASK            (1<<MINUS_PIN | 1<<PLUS_PIN | 1<<OPT_PIN)

#define KEY_CYCLE           10                      // 10 ms
#define KEY_SHORT           (50 / KEY_CYCLE)        // 50 ms
#define KEY_LONG            (2000 / KEY_CYCLE)      // 2 s
#define KEY_DOUBLE          (250 / KEY_CYCLE)       // 250 ms
#define SHOWTIME            (5000 / KEY_CYCLE)      // 5 s

#define OPT_BRIGHT          0
#define OPT_CONTRAST        1
#define OPT_NIGHT           2
#define OPT_ROTVID          3
#define OPT_VFLIP           4
#define OPT_HFLIP           5
#define NUM_OPTS            6
#define DEFAULT_OPT         OPT_BRIGHT

#define IFU_ROTVID          (1<<8)                  // internal function, rotate video

class Keys
{
    public:
        static void init();
        static void handle();        
        
        static bool showTime() { return showCnt; }
        static const char* getOptionCaption();
        static char* getOptionValueText();
        static uint8_t getOptionValue(uint8_t o);

    private:
        static void keyShort(uint32_t keys);
        static void keyLong(uint32_t keys);
        static void keyDouble(uint32_t keys);

        static class Option* option[];
        static uint8_t opt;
        static uint32_t keyCyc;                     // key cycle
        static uint8_t keyPt;                       //     press time   in cycles
        static uint8_t keyPc;                       //           counter
        static uint8_t keyRt;                       //     release time
        static uint32_t keys;                      
        static uint16_t showCnt;
};
