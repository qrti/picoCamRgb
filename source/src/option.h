// cam_wiz rgb V0.8 220330 qrt@qland.de

#pragma once

#include <string.h>
#include "pico/stdlib.h"
#include "cam.h"
#include "oled.h"

class Option
{
    public:
        Option();
        Option(const char* caption, uint16_t cmd, uint8_t def);
               
        uint8_t getValue() { return value; }
        uint8_t getDefault() { return def; }
        uint8_t getValueCam() { return Cam::readRegister(cmd); }
        void setValueCam(uint8_t v) { Cam::writeRegister(cmd, v); }
        const char *getCaption() { return caption; }
        void show(bool v) { visible = v; }
        bool isVisible() { return visible; }
        void setText(const char* text) { this->text = text; }
        char* getValueText();

        virtual void changeValue(int8_t c) { };

    protected:
        uint16_t cmd;
        uint8_t value;
        uint8_t def;
        bool visible;

    private:
        const char* text;
        const char* caption;
};

class FlagOption : public Option
{
    public:
        FlagOption(const char* caption, uint16_t cmd, uint8_t def, uint8_t mask); 

        void changeValue(int8_t c);
        void setValue();     
        
    private:
        uint8_t mask;
};

class ValueOption : public Option
{
    public:
        ValueOption(const char* caption, uint16_t cmd, uint8_t def, uint8_t min, uint8_t max, uint8_t step);

        void changeValue(int8_t c);

    private:
        uint8_t min, max, step;
};
