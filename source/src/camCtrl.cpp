// cam_wiz rgb V0.8 220330 qrt@qland.de

#include "camCtrl.h"

void CamCtrl::init()
{
    i2c_init(i2c_inst, 400*1000);
    
    gpio_set_function(PICO_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_I2C_SDA_PIN);
    gpio_pull_up(PICO_I2C_SCL_PIN);
}

void CamCtrl::writeRegister(uint8_t i2cAd, uint8_t reg, uint8_t data)
{
    uint8_t buf[2] = { reg, data };
    i2c_write_blocking(i2c_inst, i2cAd, buf, 2, false);
}

uint8_t CamCtrl::readRegister(uint8_t i2cAd, uint8_t reg)
{
    uint8_t buf[1] = { 0 };
    i2c_write_blocking(i2c_inst, i2cAd, &reg, 1, false);           
    i2c_read_blocking(i2c_inst, i2cAd, buf, 1, false);
    return buf[0];
}

void CamCtrl::scanAds()
{
    uint8_t reg = 0;

    printf("\n");

    for(int16_t a=0; a<0x80; a++){
        printf("%02x: ", a);

        if(i2c_write_blocking(i2c_inst, a, &reg, 1, false) != PICO_ERROR_GENERIC)
            printf("*\n");
        else
            printf("\n");

        sleep_ms(20);
    }
}
