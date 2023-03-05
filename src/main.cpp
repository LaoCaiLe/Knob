#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include "TFT_eSPI.h" 
#include "motor.h"

#include "lcd.h"

float target_position = 0;


Motion motion;
Screen screen;

Commander command = Commander(Serial);

void doTarget(char* cmd) { command.scalar(&motion.target, cmd); }
void doTarget1(char* cmd) { command.scalar(&motor.PID_velocity.P, cmd); }
void doTarget2(char* cmd) { command.scalar(&motor.PID_velocity.I, cmd); }
void doTarget3(char* cmd) { command.scalar(&motor.P_angle.P, cmd); }
void doTarget4(char* cmd) { command.scalar(&motor.P_angle.I, cmd); }
void doTarget5(char* cmd) { command.scalar(&motor.PID_velocity.output_ramp, cmd); }
void doTarget6(char* cmd) { command.scalar(&motor.P_angle.output_ramp, cmd); }
void doTarget7(char* cmd) { command.scalar(&motor.PID_velocity.limit, cmd); }

void TaskOnMotor(void* params)
{
	motion.task_motor();
	
}
void TaskOnTFT(void* params)
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
	xTaskCreatePinnedToCore(TaskOnTFT, "TaskOnTFT", 2048, NULL, 1, NULL, 0);
	
	command.add('T', doTarget, "target angle");
	command.add('P', doTarget1, "target angle1");
	command.add('I', doTarget2, "target angle2");
	command.add('p', doTarget3, "target angle3");
	command.add('i', doTarget4, "target angle4");
	command.add('R', doTarget5, "target angle5");
	command.add('r', doTarget6, "target angle41");
	command.add('L', doTarget7, "target angle6");
}

void loop() {
	command.run();
	// Serial.printf("target = %.3f\n" , motion.target);
	_delay(200);
}