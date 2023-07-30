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
    bool is_init = false;
};

void lv_knob_scene_init();
void lv_knob_reflash();
extern Screen screen;

#endif
