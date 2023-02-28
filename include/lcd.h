#ifndef _LCD_H
#define _LCD_H

#include "TFT_eSPI.h" // Hardware-specific library

extern TFT_eSPI tft;

class Screen
{
    public:
    void  fillArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour);
    void  ArcPro(float weizhi, float total);
    void  Pointer( float angle );
    void  SreenUpdate(bool state, float zone ,int step);
};
void lcd_init();

#endif