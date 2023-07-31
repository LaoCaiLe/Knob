#include "led.h"
#include "scale.h"

extern Scale scale;
extern int option_index;
extern int max_option_index;
void ColorLed::task()
{
    static bool button = true;
    while(1)
    {
        // if(!scale.is_Init_Thresh)
        // {
        //     static long all = 0;
        //     static char i = 0;
        //     all += scale.read();
        //     i++;
        //     if(i==20)
        //     {
        //         all = 0;
        //     }
        //     if(i==70)
        //     {
        //         scale.threshold = all / 50 -100;
        //         // Serial.printf("threshold = %d\n", scale.threshold);
        //         scale.is_Init_Thresh = true;
        //     }
        // }
        // else
        {

            if(pin == -1 || count == -1)
                return;
            // Serial.printf("value:%d\n", scale.press_value);
            
            if(button&&scale.is_press())
            {
                set_color(100, BLUE, FX_MODE_STATIC);
                button = false;
                option_index++;
                option_index %= max_option_index;
            }
            else if(!scale.is_press())
            {
                set_color(100, RED, FX_MODE_STATIC);
                button = true;
            }
            this->service();
            vTaskDelay(200);
        }

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

