#ifndef _MOTOR_H
#define _MOTOR_H

#include <SimpleFOC.h>
#include <BleKeyboard.h>

#define MOTOR_2804
// #define MOTOR_4008

#define MOTOR_EN1_PIN   12
#define MOTOR_EN2_PIN   27
#define MOTOR_EN3_PIN   25

#define MOTOR_PWM1_PIN  13
#define MOTOR_PWM2_PIN  14
#define MOTOR_PWM3_PIN  26

#define SENSOR_SDA_PIN  16
#define SENSOR_SCL_PIN  17


enum Motor_Rotation_Type
{
    ROTATION_TYPE_NONE,
    ROTATION_TYPE_TAP,
    ROTATION_TYPE_DRAG,
};

enum Motor_Drag_Lv
{
    MOTOR_STRENGTH_LEVEL_NONE,
    MOTOR_STRENGTH_LEVEL_LOW,
    MOTOR_STRENGTH_LEVEL_HIGH,
};
struct motor_option_s{
    int angle_begin;
    int angle_end;
    int range;
    char show_text[100];
    Motor_Drag_Lv motor_shake_lv;
    Motor_Rotation_Type motor_ratotion_type;

};


class Motion{
public:
    int zero_angle;
    int now_angle;
    float target_angle;
    int real_angle;
    float err_angle;
    struct motor_option_s motor_option;

    int now_range;
    int cur_angle_begin;
    int cur_angle_end;

    void init(void);
    void task_motor(void);
    void motor_run(void);
    void position_check(void);
    void shake_mode(void);
};

extern BLDCMotor motor;
extern BLDCDriver3PWM driver;
extern MagneticSensorI2C sensor;
// extern BleKeyboard bleKeyboard;
extern Motion motion;
void pulsation(int16_t time, float Amplitude);
#endif