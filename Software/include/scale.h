#ifndef _SCALE_H
#define _SCALE_H

#include <HX711.h>

class scale : public HX711
{
public:
    scale();
    ~scale();
    scale(byte pin_dout, byte pin_sck);
    bool is_press();
private:
    int max_threshold=0;
};

#endif