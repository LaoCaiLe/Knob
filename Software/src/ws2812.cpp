#include "ws2812.h"

void RGBLED::init()
{
	FastLED.addLeds<WS2812, RGB_LED_PIN, GRB>(color_buff, RGB_LED_NUM);
}

void RGBLED::setRGB(int id, int r, int g, int b)
{
	color_buff[id] = CRGB(r, g, b);
	FastLED.show();

}

void RGBLED::setBrightness(float duty)
{
	duty = constrain(duty, 0, 1);
	FastLED.setBrightness((uint8_t)(255 * duty));
	FastLED.show();

}