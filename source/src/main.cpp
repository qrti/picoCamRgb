// cam_wiz rgb V0.8 220330 qrt@qland.de
// Camera OV7725 -> RB2040 Pico -> OLED Display SSD1306

/*             ____ main ____
             /         \      \ 
          cam         oled      keys
         /   \          |        |
    camCtrl  camData   text    option

    core 0
    cam data    -> PIO -> DMA -> line buffer -> video buffer
    RGB 565                      480x2 byte     128x128x2 byte
    480x480 pix                  1 line         128 lines
    
    -> VSYNC IR -> reset line counter
    -> HREF IR  -> advance line counter, start DMA
    -> 10 ms key polling routine for options

    core 1
    line buffer       -> video buffer  -> SPI   -> OLED display
    128x2 byte       |   128x128x2 byte   30 MHz   128x128 pix RGB
                     |
    option overlay ->
*/

// options (overlay display)
// - brightness, contrast
// - night vision
// - rotate 0-90-180-270 degrees
// - vertical-flip, horizontal-flip
// - options are stored to flash

// keys
// - option:    short = next option, double = previous option
// - plus:      next value
// - minus:     previous value

// remarks
// - camera OV7725 no fifo version (without RAM on PCB) is used for this project
// - if the picture is distorted reduce OLED_SPIFREQ in oled.h 
//   or reduce/raise SYS_CLK_DIV in camData.h
// - night vision might not switch off properly, reason unknown
// - attached python scripts invoke the RP2040 assembler on .pio file changes when compiling
//   see platform.ini, pre_script.py and post_script.py
// - commented out quart.h is for simple debugging with printf on serial TX (pin 1), RX (pin 2)

// developed and compiled with vsCode, platformIO and wizio-pico
// https://code.visualstudio.com/
// https://platformio.org/
// https://github.com/Wiz-IO/wizio-pico 

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
// #include "quart.h"              // printf debug, serial TX (pin 1), RX (pin 2)
#include "cam.h"
#include "oled.h"
#include "keys.h"

void setRot();
void setRot0();                     
void setRot90();                    
void setRot180();
void setRot270();
void (*setFunc)();              

void core1_entry();

volatile uint16_t nLine = 0xffff;
volatile uint8_t ccbu = 0;
uint8_t x, y;
uint16_t* p;

int main(void)
{
    stdio_init_all();

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    // QUart::init();               // printf debug
    Cam::init(480, 480);
    Oled::init();
    Oled::showRainbow();
    Keys::init();

    // while(true){                    // test
    //     sleep_ms(500);
    //     gpio_put(PICO_DEFAULT_LED_PIN, !gpio_get(PICO_DEFAULT_LED_PIN));
    // }

    multicore_launch_core1(core1_entry);
    sleep_ms(500);

    uint16_t xr, lLine;

    while(true){        
        setRot();

        if(lLine != nLine){
            lLine = nLine;

            if((lLine << 2) % 15 < 4){                          // <- l * 128 % 480 < 128
                y = (lLine << 2) / 15;
                p = (uint16_t*)CamData::capture_buf;
                x = 0; xr = 0;

                do{
                    xr += OLED_YSIZE;

                    if(xr >= Cam::yres){
                        xr -= Cam::yres;
                        (*setFunc)();
                        x++;
                    }

                    p++;
                }while(x != OLED_XSIZE);
            }
        }

        Keys::handle();
    }
}

void setRot()
{
    static uint8_t lastRot = 0xff;

    if(lastRot != Keys::getOptionValue(OPT_ROTVID)){
        lastRot = Keys::getOptionValue(OPT_ROTVID);
    
        switch(lastRot){
            case 0:
                setFunc = *setRot0;
                break;
                
            case 1:
                setFunc = *setRot90;
                break;

            case 2:
                setFunc = *setRot180;
                break;

            case 3:
                setFunc = *setRot270;
                break;
        }
    }
}

void setRot0()
{    
    if(!Keys::showTime() || y<OPTSTART_Y)   
        Oled::set(x, y, __bswap16(*p));
}

void setRot90()
{
    if(!Keys::showTime() || x<OPTSTART_Y)
        Oled::set(OLED_YSIZE-y-1, x, __bswap16(*p));
}

void setRot180()
{
    if(!Keys::showTime() || OLED_YSIZE-y-1<OPTSTART_Y)
        Oled::set(OLED_XSIZE-1-x, OLED_YSIZE-y-1, __bswap16(*p));
}

void setRot270()
{
    if(!Keys::showTime() || OLED_YSIZE-x-1<OPTSTART_Y)
        Oled::set(y, OLED_YSIZE-x-1, __bswap16(*p));
}

void core1_entry()
{
    while(true)
        Oled::display();
}
