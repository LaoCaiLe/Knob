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
    {0, 0, -1, "No Limit", MOTOR_SHAKE_LEVEL_NONE, ROTATION_TYPE_NONE},
    {0, 0, -1, "Auto Center", MOTOR_SHAKE_LEVEL_NONE, ROTATION_TYPE_TAP},
    {0, 60, 1, "Switch", MOTOR_SHAKE_LEVEL_NONE, ROTATION_TYPE_TAP},
    {0, 360, 6, "6 taps", MOTOR_SHAKE_LEVEL_NONE, ROTATION_TYPE_TAP},
    {0, 360, 10, "10 taps", MOTOR_SHAKE_LEVEL_NONE, ROTATION_TYPE_TAP},
    {0, 360, 1, "2PI Limit", MOTOR_SHAKE_LEVEL_LOW, ROTATION_TYPE_SHAKE},
    {-90, 90, 1, "PI Limit", MOTOR_SHAKE_LEVEL_MID, ROTATION_TYPE_SHAKE},
};

int max_option_index = sizeof(motor_option_t) / sizeof(struct motor_option_s);
int option_index = 0;
int last_option_index = -1;

static void pulsation(int time , float Amplitude)
{
    motor.controller = MotionControlType::torque;

    int p_time = time;
    while(p_time--)
    {
        motor.loopFOC();
        motor.move(-Amplitude);
        vTaskDelay(1);
    }
    p_time = time;
    while(p_time--)
    {
        vTaskDelay(1);
        motor.loopFOC();
        motor.move(Amplitude);
    }

    motor.move(0);
}

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

    motor.P_angle.P = 20;
    motor.P_angle.I = 0;
    motor.P_angle.D = 0.0;
    motor.P_angle.output_ramp = 1000.0;
    motor.P_angle.limit = 30;
    motor.LPF_angle.Tf = 0.01;
    err_angle = 0.005;

    motor.init();
    motor.initFOC();

    motor.PID_velocity.limit = 0.25;
}

static int meanFilterFloat(int arr[], int n)
{
    int sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }

    return sum / n;
}


void Motion::task_motor(void)
{
    int i = 0;
    bool is_enough = false;
    int arr[100] = {0};

    while(1)
    {
        sensor.update();

        arr[i++] = sensor.getAngle() / PI * 180.0f;
        if(i==sizeof(arr)/sizeof(int))
        {
            is_enough = true;
            i = 0;
        }

        if(!is_enough)
            now_angle = arr[i-1] ;
        else
            now_angle = meanFilterFloat(arr, sizeof(arr)/sizeof(int));

        // now_angle= sensor.getAngle() * 180.0f / PI;
        real_angle = now_angle - zero_angle;
        vTaskDelay(1);
        if(last_option_index != option_index)
        {
            motor_option = motor_option_t[option_index];
            pulsation(5, 0.1);
            if (screen.is_init) 
                lv_knob_reflash();
            zero_angle = sensor.getAngle()/PI*180.0f;
            last_option_index = option_index;

            cur_angle_end = motor_option.angle_begin+(motor_option.angle_end - motor_option.angle_begin) / motor_option.range ;
            cur_angle_begin = motor_option.angle_begin;
            now_range = 0;
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
    float gap_angle = (motor_option.angle_end - motor_option.angle_begin) / motor_option.range / 2;

    if((now_angle - target_angle) > gap_angle &&(target_angle-zero_angle ) < cur_angle_end)
    {
        target_angle = cur_angle_end + zero_angle;
        now_range++;
    }
    else if((now_angle -target_angle) < -gap_angle && (target_angle-zero_angle ) > cur_angle_begin)
    {
        target_angle = cur_angle_begin + zero_angle;
        now_range--;
    }

    if((now_angle - target_angle) > gap_angle && cur_angle_end < motor_option.angle_end )
    {
        cur_angle_end += gap_angle * 2;
        cur_angle_begin += gap_angle * 2;
        
    }
    else if((now_angle -target_angle) < -gap_angle && cur_angle_begin > motor_option.angle_begin)
    {
        cur_angle_end -= gap_angle * 2;
        cur_angle_begin -= gap_angle * 2;
    }

    motor.controller = MotionControlType::angle;
}

void Motion::shake_mode()
{
    static int last_zhendong = 0; //震动标志位
    target_angle = 0;

    if(real_angle>motor_option.angle_end)
    {
        motor.controller = MotionControlType::angle;
        target_angle = motor_option.angle_end + zero_angle;
    }
    else if(real_angle<motor_option.angle_begin)
    {
        motor.controller = MotionControlType::angle;
        target_angle = motor_option.angle_begin + zero_angle;
    }

    else if((real_angle%10==0) && (real_angle>motor_option.angle_begin+10) && (real_angle<motor_option.angle_end-10))
    {
        if(!last_zhendong)
        {
            last_zhendong = true;
            switch (motor_option.motor_shake_lv)
            {
                case MOTOR_SHAKE_LEVEL_LOW:
                    pulsation(8, 0.1);
                    break;
                case MOTOR_SHAKE_LEVEL_MID:
                    pulsation(10, 0.1);
                    break;
                case MOTOR_SHAKE_LEVEL_HIGH:
                    pulsation(15, 0.1);
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
}

