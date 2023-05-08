#ifndef _WS2812_H
#define _WS2812_H

#include <FastLED.h>


#define RGB_LED_NUM 1
#define RGB_LED_PIN 27


class RGBLED
{
public:
	void init();

	void setRGB(int id, int r, int g, int b);
	void setBrightness(float duty);

private:
	CRGB color_buff[RGB_LED_NUM];
};

#endif // !_WS2812_H