// cam_wiz rgb V0.8 220330 qrt@qland.de

#include "cam.h"

uint16_t Cam::xres, Cam::yres;

void Cam::init(uint16_t xres, uint16_t yres)
{
    Cam::xres = xres;
    Cam::yres = yres;

    sleep_ms(50);        
    CamCtrl::init();
    CamData::init(xres, yres);    

    // writeRegister(COM2,     0x00);       // output drive cpability 0..3

    writeRegister(COM7,     0b00000110);    // VGA, RGB565, RGB
    // writeRegister(COM10,    0b00000000);    //

    writeRegister(SDE,      0b00000111);    // enable brightness, contrast, saturation, hue control
    writeRegister(COM5,     0b01100101);    // prepare night mode    

    writeRegister(HSTART,   0xe0>>2);       // horizontal starting position      HSTART_7:0 9:2 + HREF_5:4 1:0
    writeRegister(HSIZE,    xres>>2);       //            size                   HSIZE_7:0  9:2 + HREF_1:0 1:0
    writeRegister(VSTRT,    0x0e>>1);       // vertical starting position        VSTRT_7:0  8:1 + HREF_6   0
    writeRegister(VSIZE,    yres>>1);       //          size                     VSIZE_7:0  8:1 + HREF_2   0
    writeRegister(HREF,     0x00);          // 
    writeRegister(HOutSize, xres>>2);       // horizontal output size            HOutSize_7:0 9:2 + EXHCH_1:0 1:0
    writeRegister(VOutSize, yres>>1);       // vertical                          VOutSize_7:0 9:2 + EXHCH_2   0
    writeRegister(EXHCH,    0x00);          //   

    // writeRegister(DSPAuto,  0b01011111);    // AWB auto thres off, sharpness off
    // writeRegister(AWBCtrl3, 0x22);          // AWB mode advanced

    // writeRegister(USAT, 0x40);
    // writeRegister(VSAT, 0x40);
    // writeRegister(HUECOS, 0x80);
    // writeRegister(HUESIN, 0x80);

}

void Cam::writeRegister(uint8_t reg, uint8_t data)
{
    CamCtrl::writeRegister(ADDRW, reg, data);
}    

uint8_t Cam::readRegister(uint8_t reg)
{
    return CamCtrl::readRegister(ADDRW, reg);
}    

// uint8_t Cam::RGB565_Grey(uint16_t c)
// {
//     return (((c & 0xf800)>>8) + ((c & 0x07e0)>>3) + ((c & 0x001f)<<3)) / 3;
// }

// uint8_t Cam::RGB444_Grey(uint16_t c)
// {
//     return (((c & 0x0f00)>>4) + (c & 0x00f0) + ((c & 0x000f)<<4)) / 3;
// }

// uint8_t Cam::GBR422_Grey(uint16_t c)
// {
//     return (c & 0xf000) >> 8;
// }
