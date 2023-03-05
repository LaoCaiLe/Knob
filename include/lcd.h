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

    float circle_coord[360][2];
};
void lcd_init();

extern Screen screen;

#endif
