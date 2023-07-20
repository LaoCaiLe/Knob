#include "scale.h"

scale::scale(byte pin_dout, byte pin_sck)
{
    this->begin(pin_dout, pin_sck);
}

bool scale::is_press()
{
    long recv = this->read();
    if (recv > max_threshold)
        return true;
    else
        return false;
}