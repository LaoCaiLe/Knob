#ifndef _MOTOR_H
#define _MOTOR_H

#include <SimpleFOC.h>

#define MOTOR_2804
// #define MOTOR_4008

enum motor_rotation_type
{
    ROTATION_TYPE_TAP = 0,
    ROTATION_TYPE_SHAKE,
};

enum motor_shake_level
{
    MOTOR_SHAKE_LEVEL_HIGH = 0,
    MOTOR_SHAKE_LEVEL_MID,
    MOTOR_SHAKE_LEVEL_LOW
};
#define MOTOR_EN1_PIN   12
#define MOTOR_EN2_PIN   27
#define MOTOR_EN3_PIN   25

#define MOTOR_PWM1_PIN  13
#define MOTOR_PWM2_PIN  14
#define MOTOR_PWM3_PIN  26

#define SENSOR_SDA_PIN  16
#define SENSOR_SCL_PIN  17
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
    int shake_mode(int min_angle, int max_angle, motor_shake_level shake_lv);
};

extern BLDCMotor motor;
extern BLDCDriver3PWM driver;
extern MagneticSensorI2C sensor;
extern Motion motion;
void pulsation(int16_t time, float Amplitude);
#endif