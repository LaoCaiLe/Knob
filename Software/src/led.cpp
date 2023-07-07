#include "led.h"

void ColorLed::init()
{
    WS2812FX::init();
}
void ColorLed::task()
{
    if(pin == -1 || count == -1)
        return;
    this->service();
}

void ColorLed::set_color(uint8_t Brightness, uint32_t colors[])
{
    if(pin == -1 || count == -1)
        return;
    this->setBrightness(Brightness);
    this->setSegment(0, 0, count - 1, FX_MODE_STATIC, colors, 100, false);
    this->start();
}

