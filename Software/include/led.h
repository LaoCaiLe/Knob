#ifndef LED_H
#define LED_H
#include <WS2812FX.h>

#define LED_COUNT 8
#define LED_PIN 2

class ColorLed: public WS2812FX
{
public: 
    ColorLed() = default;
    ColorLed(uint8_t _pin, uint8_t _count) : WS2812FX(_pin, _count, NEO_GBR + NEO_KHZ800), pin(_pin), count(_count){}
    ~ColorLed() = default;
    void init();
    void task();
    void set_color(uint8_t Brightness, uint32_t colors[]);

private:
    uint8_t pin = -1;
    uint8_t count = -1;

};

#endif // !LED_H