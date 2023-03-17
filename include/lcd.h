#ifndef _LCD_H
#define _LCD_H

#include "TFT_eSPI.h" // Hardware-specific library
#include "motor.h"
extern TFT_eSPI tft;

class Screen
{
public:
    void task_tft(void);
    void init(void);
};

void lv_knob_test();
extern Screen screen;

#endif
