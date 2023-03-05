#include "lcd.h"


TFT_eSPI tft = TFT_eSPI(240, 240);

unsigned  get_remainder(int val)
{
	while(val <0)
		val += 360;
	while(val>=360)
		val -= 360;
	return val;
}
void Screen::task_tft(void)
{
  char buf[10];
	static int last_angle = 1;
	
	while(1)
	{
		// sensor.update();
		// float now_radian = sensor.getSensorAngle();
		int angle = motion.now_angle - motion.zero_angle;

		// int angle = angle_err / PI * 180;
			Serial.printf("motion.now_angle[][0] = %.3f\n" , motion.now_angle);
			Serial.printf("motion.zero_agle[][1] = %.3f\n" , motion.zero_angle);
		tft.drawCentreString("             ", 90, 20, 4);
		tft.drawCentreString(itoa(motion.now_angle, buf, 10), 90, 20, 4);
		if(angle != last_angle)
		{
			tft.drawCentreString("           ", 120, 90, 7);
			
			tft.fillCircle(circle_coord[get_remainder(last_angle)][0],circle_coord[get_remainder(last_angle)][1], 3, TFT_BLACK);

			// tft.fillRect(0, 0, 240, (6.28-radian_err) / 6.28 * 240, TFT_BLACK);

			// tft.fillRect(0, (6.28-radian_err) / 6.28 * 240, 240, 240, TFT_RED);
			
			tft.drawCentreString(itoa(angle, buf, 10), 120, 90, 7);
			tft.fillCircle(circle_coord[get_remainder(angle)][0], circle_coord[get_remainder(angle)][1], 3, TFT_GREEN);
			
			last_angle = angle;
		}
		vTaskDelay(50);
	}
}
void Screen::init(void)
{

	int i = 0;
	float radian;

	tft.init();
	tft.fillScreen(TFT_BLACK);
	tft.setRotation(0);

	for (i = 0; i < 360; i++)
	{
		radian = i * PI/ 180.0f ;
		circle_coord[i][0] = 115 * sin(radian) + 120;
		circle_coord[i][1] = 115 * -cos(radian) + 120;
		// Serial.printf("circle_coord[][0] = %.3f\n" , circle_coord[i][0]);
		// Serial.printf("circle_coord[][1] = %.3f\n" , circle_coord[i][1]);
	}
}