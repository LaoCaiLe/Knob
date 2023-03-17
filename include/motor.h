#ifndef _MOTOR_H
#define _MOTOR_H

#include <SimpleFOC.h>


#define MOTOR_EN1_PIN   12
#define MOTOR_EN2_PIN   27
#define MOTOR_EN3_PIN   25

#define MOTOR_PWM1_PIN  13
#define MOTOR_PWM2_PIN  14
#define MOTOR_PWM3_PIN  26

class Motion{
public:
    int zero_angle;
    int now_angle;
    float target_angle;
    int real_angle;
    float err_angle;
    float motor_time;
    float motor_N;
    float m_min_angle;
    float m_max_angle;
    void init();
    void task_motor(void);
    void position_check(int min_angle, int max_angle, int count);
    int fn31(u_int16_t Floors, float Floors_Zone_Du, int Touch_Feedback_Strength);
};

extern BLDCMotor motor;
extern BLDCDriver3PWM driver;
extern MagneticSensorI2C sensor;
extern Motion motion;
void pulsation(int16_t time, float Amplitude, int16_t count);
#endif