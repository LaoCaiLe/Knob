// Open loop motor control example
#include <SimpleFOC.h>
#include "motor.h"
#include "oled.h"

BLDCMotor motor = BLDCMotor(7,11);
// BLDCMotor motor = BLDCMotor(11,12.5);
BLDCDriver3PWM driver = BLDCDriver3PWM(MOTOR_PWM1_PIN, MOTOR_PWM2_PIN, MOTOR_PWM3_PIN, MOTOR_EN1_PIN, MOTOR_EN2_PIN, MOTOR_EN3_PIN);
MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);
TwoWire I2Cone = TwoWire(1);


void Motion::init()
{
    I2Cone.begin(16, 17, 400000UL);
	sensor.init(&I2Cone);
	motor.linkSensor(&sensor);	


	driver.voltage_power_supply = 12;
	driver.init();
	motor.linkDriver(&driver);


	motor.current_limit = 0.3;
    // motor.voltage_limit = 12;
    motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
    motor.torque_controller = TorqueControlType::voltage;
    motor.controller = MotionControlType::angle;
  	motor.sensor_direction = Direction::CCW;
	// motor.zero_electric_angle  = 5.3490;
	motor.zero_electric_angle  = 2.5173;

	// motor.PID_velocity.P = 0.1;
	// motor.PID_velocity.I = 4;
    motor.PID_velocity.P = 0.008;
    motor.PID_velocity.I = 0.2;
	motor.PID_velocity.D = 0.000;
	motor.PID_velocity.output_ramp = 1000;
	motor.LPF_velocity.Tf = 0.1;
	// motor.PID_velocity.limit = 0.2;

	// motor.P_angle.P = 20;
	motor.P_angle.P = 10;
	motor.P_angle.I = 0;
  	motor.P_angle.D = 0.0;
	motor.P_angle.output_ramp = 1000.0;
	motor.P_angle.limit = 60;
	motor.LPF_angle.Tf = 0.01;

  

	// 初始化电机
	motor.init();
	motor.initFOC();
    Serial.begin(115200);

    motor.PID_velocity.limit = 0.1;

	zero_angle = sensor.getAngle()/PI*180.0f;
    target_angle = zero_angle;
    err_angle = 0.005;
}

void Motion::task_motor(void)
{
    motor.controller = MotionControlType::angle;    
	
	while(1)
	{
		// sensor.update();        
        now_angle = sensor.getAngle()/PI*180.0f;
        real_angle  = now_angle - zero_angle;
		// vTaskDelay(1);
        // position_check(-120,120,10);
        // shake_mode(-145, 145, MOTOR_SHAKE_LEVEL_LOW);
        motor.loopFOC();
        // if( motor.controller == MotionControlType::angle)
        // {
        //     // if(target_angle>0 && err_angle<0)
        //     //     err_angle = -err_angle;
        //     // if(target_angle<0 && err_angle>0)
        //     //     err_angle = -err_angle;
        //     motor.move(-(target_angle / 180.0f * PI+err_angle ));
        // }
        // else
            motor.move(target_angle);
    }
  
}

void Motion::position_check(int min_angle, int max_angle, int count)
{
    m_min_angle = min_angle;
    m_max_angle = max_angle;
    int angle_range = max_angle - min_angle;
    while(angle_range%count)
        count--;
    float gap_angle = angle_range/count/2;
 
    if((now_angle -target_angle) > gap_angle)
    {
    	digitalWrite(2, HIGH);
        if((target_angle-zero_angle) < max_angle)
            target_angle += angle_range/count;

       
    }
    else if((now_angle -target_angle) < -gap_angle)
    {
	    digitalWrite(2, HIGH);
        if((target_angle-zero_angle) > min_angle)
            target_angle -= angle_range/count;

        
    }
    else
	    digitalWrite(2, LOW);

}


void pulsation(int time , float Amplitude)
{

    int p_time = time; 
    while(p_time--)
    {
        motor.loopFOC();
        motor.move(Amplitude);
    }
    p_time = time;
    while(p_time--)
    {
        motor.loopFOC();
        motor.move(-Amplitude);
    }
   
    motor.move(0);
}
int Motion::shake_mode(int min_angle, int max_angle, motor_shake_level shake_lv)
{    
    m_min_angle = min_angle;
    m_max_angle = max_angle;
    static int last_zhendong = 0; //震动标志位
    target_angle = 0;
    //  motor.controller = MotionControlType::torque;
    if(real_angle<min_angle)
    {
        motor.controller = MotionControlType::angle;
        // target_angle = -((min_angle+zero_angle) / 180.0f * PI );
        target_angle = min_angle+zero_angle;
        Serial.printf("target = %.3f", target_angle);

        // if(target_angle>0 && err_angle<0)
        //     err_angle = -err_angle;
        // if(target_angle<0 && err_angle>0)
        //     err_angle = -err_angle;

        // target_angle-=err_angle;

    }
    else if(real_angle>max_angle)
    {
        motor.controller = MotionControlType::angle;
        // target_angle = max_angle;
        target_angle = max_angle-zero_angle;
        // target_angle = -((max_angle-zero_angle) / 180.0f * PI );
        Serial.printf("target = %.3f", target_angle);

        // if(target_angle>0 && err_angle<0)
        //     err_angle = -err_angle;
        // if(target_angle<0 && err_angle>0)
        //     err_angle = -err_angle;
        // target_angle-=err_angle;
    }
    else if((real_angle%10==0)&& real_angle!=min_angle &&real_angle!=max_angle)
    {
        if(!last_zhendong)
        {
           
            last_zhendong = true;
            switch (shake_lv)
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
    // motor.loopFOC();
    // motor.move(target_angle);
    return 0 ;
}
