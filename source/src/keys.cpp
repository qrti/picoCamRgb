// cam_wiz rgb V0.8 220330 qrt@qland.de

#include "keys.h"

uint8_t Keys::opt = DEFAULT_OPT;
uint8_t Keys::keyPt;
uint8_t Keys::keyPc;
uint8_t Keys::keyRt;
uint32_t Keys::keyCyc;
uint32_t Keys::keys;
Option *Keys::option[NUM_OPTS];
uint16_t Keys::showCnt;

void Keys::init()
{
    gpio_init_mask(KEY_MASK);                                   // init key GPIOs
    gpio_set_dir_in_masked(KEY_MASK);                           // input
    gpio_set_pulls(MINUS_PIN, true, false);                     // pullup
    gpio_set_pulls(PLUS_PIN, true, false);                      // pullup
    gpio_set_pulls(OPT_PIN, true, false);                       // pullup

    Flash::init(NUM_OPTS);

    option[OPT_BRIGHT]   = new ValueOption("BRIGHT:",  BRIGHT,     0x00, 0, 255, 8);
    option[OPT_CONTRAST] = new ValueOption("CONTRA:",  CNST,       0x40, 0, 255, 8);    
    option[OPT_NIGHT]    = new  FlagOption("NIGHT: ",  COM5,       0<<7, 1<<7);
    option[OPT_ROTVID]   = new ValueOption("ROTVID:",  IFU_ROTVID, 0,    0, 3, 1);
    option[OPT_VFLIP]    = new  FlagOption("VFLIP: ",  COM3,       0<<7, 1<<7);
    option[OPT_HFLIP]    = new  FlagOption("HFLIP: ",  COM3,       0<<6, 1<<6);    

    if(!Flash::isValid())
        Flash::flash();

    option[OPT_NIGHT]->setText("OFF\0ON \0");
    option[OPT_VFLIP]->setText("OFF\0ON \0");
    option[OPT_HFLIP]->setText("OFF\0ON \0");    
}

void Keys::handle()
{
    if(time_us_32() > keyCyc){
        uint32_t k = gpio_get_all() & KEY_MASK;                 // get keys

        if(k != KEY_MASK){                                      // key pressed
            if(keyPt == 0)                                      // fresh key press
                keys = k;                                       // store keys

            if(++keyPt == 0xff)                                 // advance and restrict key press time
                keyPt = 0xff;
            
            if(keyPt == KEY_LONG)                               // key long press
                keyLong(keys);
        
            keyRt = 0;                                          // reset key release time
        }
        else{                                                   // key released
            if(++keyRt == 0xff)                                 // advance and restrict key release time
                keyRt = 0xff;

            if(keyPt<KEY_LONG && keyPt>KEY_SHORT)               // if valid key press
                keyPc++;                                        // count key press

            if(keyRt>KEY_DOUBLE && keyPc==1){                   // single short press
                keyShort(keys);                             
                keyPc = 0;                                      // reset key press counter
            }
            else if(keyRt>KEY_SHORT && keyPc==2){               // double press
                keyDouble(keys);
                keyPc = 0;                                      // reset key press counter
            }

            keyPt = 0;                                          // reset key press time
        }

        if(showCnt){                                            // showCnt
            if(--showCnt == 0){                                 // save values in flash if showCnt is down
                for(uint8_t o=0; o<NUM_OPTS; o++)               
                    Flash::setValue(o, getOptionValue(o));

                Flash::flash();    
            }
        }

        keyCyc = time_us_32() + KEY_CYCLE * 1000;               // 10 ms
    }
}

void Keys::keyShort(uint32_t keys)                              // after releasing key
{    
    if(!(keys & 1<<OPT_PIN)){  
        if(showCnt){
            uint8_t o;                                          // find next visible option
            
            for(o=1; o<NUM_OPTS; o++)
                if(option[(opt + o) % NUM_OPTS]->isVisible())
                    break;

            opt = (opt + o) % NUM_OPTS;
        }
    }
    else if(!(keys & 1<<MINUS_PIN)){
        option[opt]->changeValue(-1);
    }
    else if(!(keys & 1<<PLUS_PIN)){
        option[opt]->changeValue(1);
    }

    showCnt = SHOWTIME;
}    

void Keys::keyLong(uint32_t keys)                               // after key was pressed for 2 s
{
}

void Keys::keyDouble(uint32_t keys)                             // after key was released twice in 250 ms
{    
    if(!(keys & 1<<OPT_PIN)){  
        if(showCnt){
            uint8_t o;                                          // find previous visible option
            
            for(o=1; o<NUM_OPTS; o++)
                if(option[opt<o ? NUM_OPTS-o: opt-o]->isVisible())
                    break;

            opt = opt<o ? NUM_OPTS-o: opt-o;
        }
    }

    showCnt = SHOWTIME;
}

const char* Keys::getOptionCaption() 
{ 
    return option[opt]->getCaption(); 
}

char* Keys::getOptionValueText() 
{ 
    return option[opt]->getValueText(); 
}

uint8_t Keys::getOptionValue(uint8_t o) 
{ 
    return option[o]->getValue(); 
} 
