#include <Arduino.h>
#include <Wire.h>
#include "TFT_eSPI.h" // Hardware-specific library
// #include "as5600.h"
#include "oled.h"
// #include "mt6701.h"
#include "motor.h"



// BLDCMotor motor = BLDCMotor(7,11);
BLDCMotor motor = BLDCMotor(11,12.5);
// BLDCDriver3PWM driver = BLDCDriver3PWM(pwmA, pwmB, pwmC, Enable(optional));
BLDCDriver3PWM driver = BLDCDriver3PWM(MOTOR_PWM1_PIN, MOTOR_PWM2_PIN, MOTOR_PWM3_PIN, MOTOR_EN1_PIN, MOTOR_EN2_PIN, MOTOR_EN3_PIN);
MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);
TwoWire I2Cone = TwoWire(1);
OLED oled;
// TFT_eSPI lcd = TFT_eSPI(240,240);

void task_motor(void* params)
{
  while(1)
  {
    motor.move(6.28);
    motor.loopFOC();
    vTaskDelay(2);
  }
  
}
void setup() {

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  I2Cone.begin(16, 17, 400000UL);
  sensor.init(&I2Cone);
  motor.linkSensor(&sensor);

  oled.begin();
  // lcd.begin();
  // lcd.fillScreen(TFT_GREEN);
  // lcd.setRotation(2);

  // 配置驱动器
  // 电源电压 [V]
  driver.voltage_power_supply = 8;
  driver.init();
  // 连接电机和驱动器
  motor.linkDriver(&driver);

  // 限制电机电流 （电阻给定的话）
  motor.current_limit = 0.5;
  // motor.voltage_limit = 3;
  // 配置开环控制
  // motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
  motor.controller = MotionControlType::velocity;

  //  // 设置力矩控制类型
  // motor.torque_controller = TorqueControlType::voltage;
  // // 设置运动控制环
  // motor.controller = MotionControlType::torque;

  motor.PID_velocity.P = 0.02;
  motor.PID_velocity.I = 2;
  motor.PID_velocity.D = 0.000;
  // 基于斜坡电压的急动控制
  // 默认值为300v/s，即0.3v/ms
  motor.PID_velocity.output_ramp = 1000;
  motor.LPF_velocity.Tf = 0.01;
  //  motor.velocity_limit = 6;

  // motor.P_angle.P = 5;
  // 初始化电机
  motor.init();
  motor.initFOC();
 

  Serial.begin(9600);
  Serial.println("Motor ready!");

  _delay(1000);
  digitalWrite(2, LOW);

  xTaskCreatePinnedToCore(task_motor, "task_motor", 2048, NULL, 1, NULL, 0);
}
// long timestamp_us = _micros();
void loop() {

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