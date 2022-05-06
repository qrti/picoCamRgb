// cam_wiz rgb V0.8 220330 qrt@qland.de

#include "flash.h"

uint8_t* Flash::flashBuf;
uint8_t Flash::size;
bool Flash::valid;
uint8_t Flash::index;

void Flash::init(uint8_t _size)
{
    size = _size;

    flashBuf = (uint8_t*)malloc(FLASH_PAGE_SIZE);
    memcpy(flashBuf, (uint8_t*)(XIP_BASE + FLASH_TARGET_OFFSET), FLASH_PAGE_SIZE);
    valid = checkFletcher();
}

uint8_t Flash::getNextValue(uint8_t def)
{
    uint8_t v;

    if(valid){
        v = flashBuf[index++];
    }
    else{
        v = def;
        flashBuf[index++] = def;
    }

    return v;
}

void Flash::setValue(uint8_t ad, uint8_t v)
{
    flashBuf[ad] = v;
}

void Flash::flash()
{
    addFletcher();

    uint32_t status = save_and_disable_interrupts();
    // multicore_reset_core1();
    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
    flash_range_program(FLASH_TARGET_OFFSET, flashBuf, FLASH_PAGE_SIZE);
    // multicore_launch_core1(core1_entry);
    restore_interrupts(status);
}

void Flash::addFletcher()
{
    uint8_t a0=0, a1=0;
   
    for(uint8_t b=0; b<size; b++){
        a0 = (uint8_t)(a0 + flashBuf[b] + b+1) % 0xff;
        a1 = (uint8_t)(a0 + a1) % 0xff;
    }

    uint8_t c0 = 0xff - (uint8_t)(a0 + a1) % 0xff;
    uint8_t c1 = 0xff - (uint8_t)(a0 + c0) % 0xff;

    flashBuf[size] = c0;
    flashBuf[size + 1] = c1;
}

bool Flash::checkFletcher()
{
    uint8_t a0=0, a1=0;
   
    for(uint8_t b=0; b<size+2; b++){
        a0 = (uint8_t)(a0 + flashBuf[b] + (b<size ? b+1 : 0)) % 0xff;
        a1 = (uint8_t)(a0 + a1) % 0xff;
    }

    return a0==0 && a1==0;
}
