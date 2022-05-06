// cam_wiz rgb V0.8 220330 qrt@qland.de

#include "oled.h"
#include "math.h"

uint16_t* Oled::vidBuf;              
uint8_t Oled::oledX, Oled::oledY;

void Oled::init()
{
    vidBuf = (uint16_t*)malloc(VIDSIZE*2);

    gpio_init(OLED_RST_PIN);         
    gpio_set_dir(OLED_RST_PIN, GPIO_OUT);          
    reset();

    spiInit();
    sleep_ms(1);

    uint8_t* p = (uint8_t*)initSeq;
    uint8_t len;

    while((len=*p++) != 255){
        spiWriteCmd(*p, p+1, len);
        p += len + 1;
    }

    sleep_ms(200);
    spiWriteCmd(0xaf, 0, 0);            // display on

    clear();                            // clear buffer
    show();                             // show
}

void Oled::display()
{        
    if(Keys::showTime()){
        char buf[17];
        sprintf(buf, "%s %s", Keys::getOptionCaption(), Keys::getOptionValueText());
        clrLine(OPTSTART_Y, 4);
        Text::writeText(buf, 0, OPTSTART_Y+4);
        fillRight(Text::getPosX(), OPTSTART_Y+4, 14);
    }

    show();
}

void Oled::set(uint8_t x, uint8_t y, uint16_t color)
{
    *(vidBuf + (y<<7) + x) = color;
}

void Oled::clrLine(uint8_t ys, uint8_t h)
{
    for(uint8_t y=ys; y<ys+h; y++)
        for(uint8_t x=0; x<OLED_XSIZE; x++)
            set(x, y, OPTCOL_BACK);
}

void Oled::fillRight(uint8_t xs, uint8_t ys, uint8_t h)
{
    for(uint8_t y=ys; y<ys+h; y++)
        for(uint8_t x=xs; x<OLED_XSIZE; x++)
            set(x, y, OPTCOL_BACK);
}

void Oled::showRainbow()
{    
    float tx = 0;
    float ty = 1.0f;
    float ax = 1.0f / OLED_XSIZE;
    float ay = 1.0f / OLED_YSIZE;

    for(uint8_t y=0; y<OLED_YSIZE; y++){
        tx = 0;

        for(uint8_t x=0; x<OLED_XSIZE; x++){
            set(x, y, col565(tx, ty));
            tx += ax;
        }

        ty -= ay;
    }

    show();
}

#define X0      0.0f
#define X1      (1.0f / 6.0f)
#define X2      (2.0f / 6.0f)
#define X3      0.5f
#define X4      (4.0f / 6.0f)
#define X5      (5.0f / 6.0f)

uint16_t Oled::col565(float x, float fy)
{
    uint16_t red = noc(x,      fy, 31, 11);    
    uint16_t gre = noc(x + X4, fy, 63, 5);
    uint16_t blu = noc(x + X2, fy, 31, 0);

    return __bswap16(red | gre | blu);
}

uint16_t Oled::noc(float x, float fy, uint8_t maxc, uint8_t shift)
{
    float fx;

    if(x >= 1.0f) x -= 1.0f;

    if     (x < X1) { fx = 1.0f; }                  // 1 -> 1
    else if(x < X2) { fx = 1.0f - (x - X1) / X1; }  // 1 -> 0
    else if(x < X3) { fx = 0.0f; }                  // 0 -> 0
    else if(x < X4) { fx = 0.0f; }                  // 0 -> 0
    else if(x < X5) { fx = (x - X4) / X1; }         // 0 -> 1
    else            { fx = 1.0f; }                  // 1 -> 1

    return (uint16_t)round(fx * fy * maxc) << shift;
}

void Oled::clear()
{      
    uint16_t *p = vidBuf;

    while(p < vidBuf+VIDSIZE)
        *p++ = 0;
}

void Oled::show()
{   
static uint8_t sten[] = { 0, 127 };

    spiWriteCmd(0x15, sten, 2);             // column start end
    spiWriteCmd(0x75, sten, 2);             // row
    spiWriteCmd(0x5c, 0, 0);                // enable write data
    spiWriteData((uint8_t*)vidBuf, VIDSIZE*2);    
}

void Oled::reset()
{
    gpio_put(OLED_RST_PIN, 1);                        
    sleep_ms(100);
    gpio_put(OLED_RST_PIN, 0);
    sleep_ms(100);
    gpio_put(OLED_RST_PIN, 1);                        
    sleep_ms(100);
}

void Oled::spiInit()
{
    spi_init(spi_inst, OLED_SPIFREQ);
    
    gpio_set_function(OLED_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(OLED_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(OLED_SPI_CS_PIN, GPIO_FUNC_SPI);

    gpio_init(OLED_SPI_DC_PIN);         
    gpio_set_dir(OLED_SPI_DC_PIN, GPIO_OUT);          
    gpio_put(OLED_SPI_DC_PIN, OLED_DATA);     
} 

void Oled::spiWriteCmd(uint8_t reg, uint8_t* buf, size_t len)
{
    gpio_put(OLED_SPI_DC_PIN, OLED_CMD);
    spi_write_blocking(spi_inst, &reg, 1);

    if(len){
        gpio_put(OLED_SPI_DC_PIN, OLED_DATA);
        spi_write_blocking(spi_inst, buf, len);
    }
}

void Oled::spiWriteData(uint8_t* buf, size_t len)
{
    gpio_put(OLED_SPI_DC_PIN, OLED_DATA);
    spi_write_blocking(spi_inst, buf, len);
}

// -----------------------------------------------------------------------------

//void Oled::showRainbow()              // float trigonometry version
//{    
//    float tx = 0;
//    float ty = 1.0f;
//    float ax = M_PI / XSIZE;
//    float ay = 1.0f / YSIZE;
//
//    for(uint8_t y=0; y<YSIZE; y++){
//        tx = 0;
//
//        for(uint8_t x=0; x<XSIZE; x++){
//            set(x, y, col565(tx, ty));
//            tx += ax;
//        }
//
//        ty -= ay;
//    }
//
//    show();
//}
//
//uint16_t Oled::col565(float x, float fac)
//{
//static float pi3 = M_PI / 3.0f;
//
//    uint16_t red = noc(x,  0.0f, fac, 31, 11);    
//    uint16_t gre = noc(x, -pi3,  fac, 63, 5);
//    uint16_t blu = noc(x,  pi3,  fac, 31, 0);
//
//    return __bswap16(red | gre | blu);
//}
//
//uint16_t Oled::noc(float x, float o, float fac, uint8_t maxc, uint8_t shift)
//{
//    float norc = 0.5f + 0.608f * cos(2.0f * (x + o)) - 0.135f * cos(6.0f * (x + o)) + 0.024f * cos(10.0f * (x + o));
//    if(norc < 0.0f) norc = 0.0f;
//    if(norc > 1.0f) norc = 1.0f;
//    return (uint16_t)round(norc * fac * maxc) << shift;
//}