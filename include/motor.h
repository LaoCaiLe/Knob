#ifndef _MOTOR_H
#define _MOTOR_H

#include <SimpleFOC.h>
// #include "BLDCMotor.h"
#define pi 3.1415926
#define init_smooth 1000 // 该值越大，初始化越慢。以防受到干扰。
#define volt_limit 8.0000

#define MOTOR_EN1_PIN   12
#define MOTOR_EN2_PIN   27
#define MOTOR_EN3_PIN   25

#define MOTOR_PWM1_PIN  13
#define MOTOR_PWM2_PIN  14
#define MOTOR_PWM3_PIN  26


void init_angle(void);
void Task_foc(void);
void motor_shake(int strength, int delay_time);

#endif