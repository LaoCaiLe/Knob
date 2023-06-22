#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include "TFT_eSPI.h" 
#include "lvgl.h"
#include "motor.h"
#include "lcd.h"
#include <WS2812FX.h>

float target_position = 0;

Motion motion;
Screen screen

#define LED_COUNT 8
#define LED_PIN 32

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

Commander command = Commander(Serial);

void doTarget(char* cmd) { command.scalar(&motion.target_angle, cmd); }
void doTarget1(char* cmd) { command.scalar(&motor.PID_velocity.P, cmd); }
void doTarget2(char* cmd) { command.scalar(&motor.PID_velocity.I, cmd); }
void doTarget3(char* cmd) { command.scalar(&motor.LPF_velocity.Tf, cmd); }
void doTarget4(char* cmd) { command.scalar(&motor.PID_velocity.output_ramp, cmd); }
void doTarget5(char* cmd) { command.scalar(&motion.err_angle, cmd); }
void doTarget6(char* cmd) { command.scalar(&motion.motor_time, cmd); }
void doTarget8(char* cmd) { command.scalar(&motion.motor_N, cmd); }
void doTarget7(char* cmd) { command.scalar(&motor.PID_velocity.limit, cmd); }

void TaskOnMotor(void* params)
{
	motion.task_motor();
}

static void lv_timer_cb(lv_timer_t *t)
{
	screen.task_tft();
}
uint32_t colors[] = {RED, GREEN,ORANGE,PURPLE };
void setup() {

	Serial.begin(115200);
	
	pinMode(2, OUTPUT);
	ws2812fx.init();
  	ws2812fx.setBrightness(50);
	ws2812fx.setSegment(0, 0, 7, FX_MODE_BREATH, colors, 1000, false);
  	ws2812fx.start();

	digitalWrite(2, HIGH);

	screen.init();
	motion.init();
	
	digitalWrite(2, LOW);

	xTaskCreatePinnedToCore(TaskOnMotor, "TaskOnMotor", 2048, NULL, 2, NULL, 0);

	lv_timer_create(lv_timer_cb, 1, NULL);
	
	// command.add('T', doTarget, "target angle");
	// command.add('P', doTarget1, "target angle1");
	// command.add('I', doTarget2, "target angle2");
	// command.add('p', doTarget3, "target angle3");
	// command.add('i', doTarget4, "target angle4");
	// command.add('R', doTarget5, "target angle5");
	// command.add('r', doTarget6, "target angle4");
	// command.add('L', doTarget7, "target angle6");
	// command.add('N', doTarget8, "target angle6");
}

void loop() 
{
	command.run();
  	ws2812fx.service();
	lv_task_handler();

}