#ifndef _SCALE_H
#define _SCALE_H

#include <HX711.h>

#define SCALE_OUT_PIN 32
#define SCALE_SCK_PIN 33

class Scale : public HX711
{

public:
    Scale() = default;
    ~Scale() = default;
    void init(byte pin_dout, byte pin_sck);
    bool is_press();
    long press_value=0;

private:
    int max_threshold=192500;

};

#endif