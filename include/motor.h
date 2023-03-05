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

class Motion{
public:
    float zero_angle;
    float now_angle;
    float target;
    void init();
    void task_motor(void);
};

extern BLDCMotor motor;
extern BLDCDriver3PWM driver;
extern MagneticSensorI2C sensor;
extern Motion motion;
void pulsation(int16_t time, float Amplitude, int16_t count);
#endif