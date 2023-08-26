#include "led.h"

extern int option_index;
extern int max_option_index;

static bool is_press()
{
    touch_value_t press_value = touchRead(T9);
    if (press_value < 45)
    {
        vTaskDelay(30);
        press_value = touchRead(T9);
        if (press_value < 45)
            return true;
        else
            return false;
    }
}

void ColorLed::task()
{
    static bool button = true;
    while(1)
    {
        if(pin == -1 || count == -1)
            return;

        if(button&&is_press())
        {
            set_color(100, BLUE, FX_MODE_STATIC);
            button = false;
            option_index++;
            option_index %= max_option_index;
        }
        else if(!is_press())
        {
            set_color(100, RED, FX_MODE_STATIC);
            button = true;
        }
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

