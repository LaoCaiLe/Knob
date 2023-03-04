#include <Arduino.h>
#include <Wire.h>
#include "TFT_eSPI.h" 
#include "motor.h"
#include <math.h>

#define PI 3.14159625
float target_position = 0;
// BLDCMotor motor = BLDCMotor(7,11);
BLDCMotor motor = BLDCMotor(11,12.5);
BLDCDriver3PWM driver = BLDCDriver3PWM(MOTOR_PWM1_PIN, MOTOR_PWM2_PIN, MOTOR_PWM3_PIN, MOTOR_EN1_PIN, MOTOR_EN2_PIN, MOTOR_EN3_PIN);
MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);
TwoWire I2Cone = TwoWire(1);
TFT_eSPI lcd = TFT_eSPI(240,240);
Commander command = Commander(Serial);

void doTarget(char* cmd) { command.scalar(&target_position, cmd); }
void doTarget1(char* cmd) { command.scalar(&motor.PID_velocity.P, cmd); }
void doTarget2(char* cmd) { command.scalar(&motor.PID_velocity.I, cmd); }
void doTarget3(char* cmd) { command.scalar(&motor.P_angle.P, cmd); }
void doTarget4(char* cmd) { command.scalar(&motor.P_angle.I, cmd); }
void doTarget5(char* cmd) { command.scalar(&motor.PID_velocity.output_ramp, cmd); }
void doTarget6(char* cmd) { command.scalar(&motor.P_angle.output_ramp, cmd); }
void doTarget7(char* cmd) { command.scalar(&motor.PID_velocity.limit, cmd); }

float target[] = {0, 2, 3.14, 6.28};

float zero_radian = 0.0f;
static int i;
void task_motor(void* params)
{
	
	while(1)
	{
		// sensor.update();
		
		motor.loopFOC();
		motor.move(-zero_radian);
		vTaskDelay(2);
	}
  
}
void task_lcd(void* params)
{
	char buf[10];
	static float last_angle = 1;
	static float last_radian_err = 1;
	
	while(1)
	{
		// sensor.update();
		float now_radian = sensor.getAngle();
		float radian_err = now_radian - zero_radian;
		int angle = radian_err / PI * 180;

		lcd.drawCentreString("             ", 90, 20, 4);
		lcd.drawFloat(now_radian, 3, 90, 20, 4);

		if(angle != last_angle)
		{
			lcd.drawCentreString("           ", 120, 90, 7);
			
			lcd.fillCircle(115 * sin(last_radian_err) + 120, 115 * -cos(last_radian_err) + 120, 3, TFT_BLACK);

			// lcd.fillRect(0, 0, 240, (6.28-radian_err) / 6.28 * 240, TFT_BLACK);

			// lcd.fillRect(0, (6.28-radian_err) / 6.28 * 240, 240, 240, TFT_RED);


			lcd.drawCentreString(itoa(angle, buf, 10), 120, 90, 7);
			lcd.fillCircle(115 * sin(radian_err) + 120, 115 * -cos(radian_err) + 120, 3, TFT_GREEN);
			last_angle = angle;
			last_radian_err = radian_err;
		}
		// lcd.drawCentreString(itoa(now_radian/ PI * 180, buf, 10), 120, 90, 7);
		vTaskDelay(20);
	}
}

void setup() {

	pinMode(2, OUTPUT);

	lcd.begin();
	lcd.fillScreen(TFT_BLACK);
	lcd.setRotation(0);

	digitalWrite(2, HIGH);
	I2Cone.begin(16, 17, 400000UL);
	sensor.init(&I2Cone);
	motor.linkSensor(&sensor);	

	// 配置驱动器
	// 电源电压 [V]
	driver.voltage_power_supply = 8;
	driver.init();
	// 连接电机和驱动器
	motor.linkDriver(&driver);

	// 限制电机电流 （电阻给定的话）
	motor.current_limit = 0.3;
	// motor.voltage_limit = 3;
	// 配置开环控制
	motor.foc_modulation = FOCModulationType::SpaceVectorPWM;

	// 设置力矩控制类型
	// motor.torque_controller = TorqueControlType::voltage;
	// 设置运动控制环
	motor.controller = MotionControlType::angle;
  	motor.sensor_direction = Direction::CCW;

	motor.zero_electric_angle  = 5.3152;
	motor.PID_velocity.P = 0.1;
	motor.PID_velocity.I = 4;
	motor.PID_velocity.D = 0.000;
	motor.PID_velocity.output_ramp = 1000;
	motor.LPF_velocity.Tf = 0.1;
	motor.PID_velocity.limit = 0.2;

	motor.P_angle.P = 10;
	motor.P_angle.I = 0;
  	motor.P_angle.D = 0.0;
	motor.P_angle.output_ramp = 5000.0;
	motor.P_angle.limit = 10;
	motor.LPF_angle.Tf = 0.01;
	// 初始化电机
	motor.init();
	motor.initFOC();

	digitalWrite(2, LOW);

	zero_radian = sensor.getAngle();

	xTaskCreatePinnedToCore(task_motor, "task_motor", 2048, NULL, 2, NULL, 0);
	xTaskCreatePinnedToCore(task_lcd, "task_lcd", 2048, NULL, 1, NULL, 0);
 	Serial.begin(115200);
	Serial.println("Sensor zero offset is:");
  Serial.println(zero_radian, 4);
	command.add('T', doTarget, "target angle");
	command.add('P', doTarget1, "target angle1");
	command.add('I', doTarget2, "target angle2");
	command.add('p', doTarget3, "target angle3");
	command.add('i', doTarget4, "target angle4");
	command.add('R', doTarget5, "target angle5");
	command.add('r', doTarget6, "target angle41");
	command.add('L', doTarget7, "target angle6");


	motor.PID_velocity.limit = 0.1;

}
// long timestamp_us = _micros();
void loop() {

	command.run();
  
// motor.loopFOC();

  // Motion control function
//   motor.move(0.2);
	// 开环速度运动
	// 使用电机电压限制和电机速度限制

	// if(_micros() - timestamp_us > 1e6) {
	//     timestamp_us = _micros();
	//     // 调换角度
	//     target_velocity = -target_velocity;   
	// }
	// motor.move(3.14 );
	// motor.loopFOC();
	// sensor.update();
	// oled.Float(0, 0, sensor.getSensorAngle(), 4);
	// 
	// Serial.printf("speed:%.04f", sensor.getVelocity());
	// if (millis() - last_update_time > 1000)
		// {
	//   oled.Float(0, 2, sensor.getVelocity(), 4);
	//   last_update_time = millis();
	// }
}