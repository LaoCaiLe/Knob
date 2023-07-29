// Open loop motor control example
#include <SimpleFOC.h>
#include "motor.h"
#include "lcd.h"
#ifdef MOTOR_2804
BLDCMotor motor = BLDCMotor(7, 5.1);
#elif define MOTOR_4008
BLDCMotor motor = BLDCMotor(11,12.5);
#endif
BLDCDriver3PWM driver = BLDCDriver3PWM(MOTOR_PWM1_PIN, MOTOR_PWM2_PIN, MOTOR_PWM3_PIN, MOTOR_EN1_PIN, MOTOR_EN2_PIN, MOTOR_EN3_PIN);
MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);
TwoWire I2Cone = TwoWire(1);

struct motor_option_s motor_option_t[] =
{
    {0, 0, -1,"auto center", MOTOR_SHAKE_LEVEL_NONE, ROTATION_TYPE_NONE},
    {0, 60, 1,"switch", MOTOR_SHAKE_LEVEL_NONE, ROTATION_TYPE_TAP },
    {0, 360, 1,"null",MOTOR_SHAKE_LEVEL_LOW, ROTATION_TYPE_SHAKE},
    {-90, 90, 1,"null",MOTOR_SHAKE_LEVEL_LOW, ROTATION_TYPE_SHAKE},
};

int option_index = 0;
int last_option_index = -1;

void Motion::init()
{
    I2Cone.begin(SENSOR_SDA_PIN, SENSOR_SCL_PIN, 400000UL);
    sensor.init(&I2Cone);

    motor.linkSensor(&sensor);

    driver.voltage_power_supply = 8;
    driver.init();
    motor.linkDriver(&driver);
#ifdef MOTOR_2804
    motor.current_limit = 0.3;
#elif define MOTOR_4008    
    motor.voltage_limit = 3;
#endif
    motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
    motor.torque_controller = TorqueControlType::voltage;
    motor.controller = MotionControlType::torque;
    motor.sensor_direction = Direction::CCW;

#ifdef MOTOR_2804
    motor.zero_electric_angle = 1.7426;
    motor.PID_velocity.P = 0.02;
    motor.PID_velocity.I = 1.0;
    motor.PID_velocity.D = 0.000;
    motor.PID_velocity.output_ramp = 1000;
    motor.LPF_velocity.Tf = 0.01;
    motor.PID_velocity.limit = 0.2;

#elif define MOTOR_4008
    motor.zero_electric_angle = 5.3152;
    motor.PID_velocity.P = 0.1;
    motor.PID_velocity.I = 5;
    motor.PID_velocity.D = 0.000;
    motor.PID_velocity.output_ramp = 1000;
    motor.LPF_velocity.Tf = 0.05;
    motor.PID_velocity.limit = 0.2;
#endif

    motor.P_angle.P = 30;
    motor.P_angle.I = 0.1;
    motor.P_angle.D = 0.0;
    motor.P_angle.output_ramp = 1000.0;
    motor.P_angle.limit = 20;
    motor.LPF_angle.Tf = 0.01;

    // 初始化电机
    motor.init();
    motor.initFOC();

    motor.PID_velocity.limit = 0.2;

    zero_angle = sensor.getAngle()/PI*180.0f;
    // target_angle = zero_angle;
    err_angle = 0.005;
    Serial.printf("zero_angle = %d\n", zero_angle);
}

void Motion::task_motor(void)
{
    int i = 0;
    bool is_1st_100=false;
    int arr[200]={0};

    while(1)
    {
        sensor.update();
        now_angle= sensor.getAngle()* 180.0f / PI ;
        // arr[i] = sensor.getAngle() / PI * 180.0f;
        real_angle = now_angle - zero_angle ;
        vTaskDelay(1);
        if(last_option_index != option_index)
        {
            motor_option = motor_option_t[option_index];
            if (screen.is_init)lv_knob_flash();
            zero_angle = sensor.getAngle()/PI*180.0f;
            last_option_index = option_index;
        }
        motor_run();
        
        motor.loopFOC();
        if( motor.controller == MotionControlType::angle)
        {
            if(target_angle>0 && err_angle<0)
                err_angle = -err_angle;
            if(target_angle<0 && err_angle>0)
                err_angle = -err_angle;
            motor.move(-(target_angle / 180.0f * PI+err_angle ));
        }
        else
            motor.move(target_angle);
    }

}

void Motion::motor_run()
{
    switch (motor_option.motor_ratotion_type)
    {
        case ROTATION_TYPE_NONE:
            target_angle = 0;
            motor.controller = MotionControlType::torque;
            break;
        case ROTATION_TYPE_TAP:
            position_check();
            break;
        case ROTATION_TYPE_SHAKE:
            shake_mode();
            break;
    }
}
void Motion::position_check()
{
    int count = motor_option.range;
   
    // motor_option_t.angle_start = min_angle;
    // motor_option_t.angle_end = max_angle;
    // motor_option = motor_option_t;
    int angle_range = motor_option.angle_end - motor_option.angle_start;
    while(angle_range % count)
        count--;
    float gap_angle = angle_range/count/2;
 
    if((now_angle -target_angle) > gap_angle)
    {
        if((target_angle-zero_angle ) < motor_option.angle_end)
        {
            target_angle = motor_option.angle_end+zero_angle;
        }
    }
    else if((now_angle -target_angle) < -gap_angle)
    {
        if((target_angle-zero_angle ) > motor_option.angle_start)
        {
            target_angle =motor_option.angle_start+zero_angle;
        }
    }

    motor.controller = MotionControlType::angle;
}

static void pulsation(int time , float Amplitude)
{
    motor.controller = MotionControlType::torque;

    int p_time = time; 
    while(p_time--)
    {
        motor.loopFOC();
        motor.move(-Amplitude);
    }
    p_time = time;
    while(p_time--)
    {
        motor.loopFOC();
        motor.move(Amplitude);
    }

    motor.move(0);
}

int Motion::shake_mode()
{

    static int last_zhendong = 0; //震动标志位
    target_angle = 0;

    if(real_angle>motor_option.angle_end)
    {
        motor.controller = MotionControlType::angle;
        target_angle = motor_option.angle_end + zero_angle  ;
        Serial.printf("target_angle %f\n", target_angle);
    }
    else if(real_angle<motor_option.angle_start)
    {
        motor.controller = MotionControlType::angle;
        target_angle = motor_option.angle_start + zero_angle  ;
        Serial.printf("target_angle %f\n", target_angle);
    }

    else if((real_angle%10==0) && (real_angle>motor_option.angle_start+10) && (real_angle<motor_option.angle_end-10))
    {
        if(!last_zhendong)
        {
            last_zhendong = true;
            switch (motor_option.motor_shake_lv)
            {
                case MOTOR_SHAKE_LEVEL_LOW:
                    pulsation(10, 0.1);
                    break;
                case MOTOR_SHAKE_LEVEL_MID:
                    pulsation(20, 0.1);
                    break;
                case MOTOR_SHAKE_LEVEL_HIGH:
                    pulsation(20, 0.2);
                    break;
                default:
                    break;
            }
        }
    }
    else
    {
        motor.controller = MotionControlType::torque;
        last_zhendong = false;
    }
    motor.loopFOC();
    return 0 ;
}

