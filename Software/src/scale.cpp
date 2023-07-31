#include "scale.h"

void Scale::init(byte pin_dout, byte pin_sck)
{
    this->begin(pin_dout, pin_sck);
}

bool Scale::is_press()
{
    // press_value = this->read();
    // if (press_value < threshold)
    //     return true;
    // else
    //     return false;
    if(touchRead(T8) < 10)
    {
        vTaskDelay(10);
        if(touchRead(T8) < 10)
            return true;
    }
    else
        return false;
}