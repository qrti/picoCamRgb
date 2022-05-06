// cam_wiz rgb V0.8 220330 qrt@qland.de

#include "option.h"

Option::Option(const char* caption, uint16_t cmd, uint8_t def)
{
    this->caption = caption;
    this->cmd = cmd;    
    this->def = def;
    this->visible = true;  
    value = Flash::getNextValue(def);
    setText(NULL);
}

char ovalt[4];

char *Option::getValueText()
{
    if(text){
        return (char *)text + (value==0) * 4;       // OFF or ON
    }
    else{
        sprintf(ovalt, "%3d", value);               // decimal value
        return ovalt;
    }
}

FlagOption::FlagOption(const char* caption, uint16_t cmd, uint8_t def, uint8_t mask) : Option(caption, cmd, def)
{
    this->mask = mask;
    setValue();
}

void FlagOption::changeValue(int8_t c) 
{ 
    value ^= mask;
    setValue();
}

void FlagOption::setValue()
{
    uint8_t v = getValueCam();

    if(value)
        v &= ~mask;
    else
        v |= mask;

    sleep_ms(1);
    setValueCam(v);
}

ValueOption::ValueOption(const char* caption, uint16_t cmd, uint8_t def, uint8_t min, uint8_t max, uint8_t step) : Option(caption, cmd, def)
{    
    this->min = min;
    this->max = max;
    this->step = step;    

    if(cmd < 0x100)                         // camera value
        setValueCam(value);
}

void ValueOption::changeValue(int8_t c) 
{ 
    int16_t v = (int16_t)value + c * step;    
    if(v < min) v = min;
    if(v > max) v = max;
    v = (v / step) * step;
    value = v;

    if(cmd < 0x100)                         // camera value
        setValueCam(value);
}
