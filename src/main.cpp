#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include "TFT_eSPI.h" 
#include "lvgl.h"
#include "motor.h"
#include "lcd.h"

float target_position = 0;

Motion motion;
Screen screen;

Commander command = Commander(Serial);

void doTarget(char* cmd) { command.scalar(&motion.target_angle, cmd); }
void doTarget1(char* cmd) { command.scalar(&motor.PID_velocity.P, cmd); }
void doTarget2(char* cmd) { command.scalar(&motor.PID_velocity.I, cmd); }
void doTarget3(char* cmd) { command.scalar(&motor.P_angle.P, cmd); }
void doTarget4(char* cmd) { command.scalar(&motor.P_angle.I, cmd); }
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
void setup() {

	Serial.begin(115200);

	pinMode(2, OUTPUT);

	digitalWrite(2, HIGH);

	screen.init();
	motion.init();
	
	digitalWrite(2, LOW);

	xTaskCreatePinnedToCore(TaskOnMotor, "TaskOnMotor", 2048, NULL, 2, NULL, 0);

	lv_timer_create(lv_timer_cb, 1, NULL);
	// lv_timer_create(lv_timer_motor_cb, 1, NULL);
	
	command.add('T', doTarget, "target angle");
	command.add('P', doTarget1, "target angle1");
	command.add('I', doTarget2, "target angle2");
	command.add('p', doTarget3, "target angle3");
	command.add('i', doTarget4, "target angle4");
	command.add('R', doTarget5, "target angle5");
	command.add('r', doTarget6, "target angle4");
	command.add('L', doTarget7, "target angle6");
	command.add('N', doTarget8, "target angle6");
}

void loop() 
{
	command.run();
	lv_task_handler();

}