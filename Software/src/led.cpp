#include "led.h"
#include "scale.h"

extern Scale scale;

void ColorLed::task()
{
    while(1)
    {
        if(pin == -1 || count == -1)
            return;

        if(scale.is_press())
            set_color(100, RED, FX_MODE_STATIC);
        else
            set_color(100, BLUE, FX_MODE_STATIC);

        vTaskDelay(5);
        this->service();

    }

}

void ColorLed::set_color(uint8_t Brightness, uint32_t colors, uint8_t mode)
{
    if(pin == -1 || count == -1)
        return;
    this->setBrightness(Brightness);
    this->setSegment(0, 0, count - 1, mode, colors, 100, false);
    this->start();
}

