// Open loop motor control example
#include <SimpleFOC.h>
#include "motor.h"
#include "oled.h"

// BLDCMotor motor = BLDCMotor(7,11);
BLDCMotor motor = BLDCMotor(11,12.5);
BLDCDriver3PWM driver = BLDCDriver3PWM(MOTOR_PWM1_PIN, MOTOR_PWM2_PIN, MOTOR_PWM3_PIN, MOTOR_EN1_PIN, MOTOR_EN2_PIN, MOTOR_EN3_PIN);
MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);
TwoWire I2Cone = TwoWire(1);


void Motion::init()
{
    I2Cone.begin(16, 17, 400000UL);
	sensor.init(&I2Cone);
	motor.linkSensor(&sensor);	


	driver.voltage_power_supply = 8;
	driver.init();
	motor.linkDriver(&driver);


	motor.current_limit = 0.3;
    // motor.voltage_limit = 3;
	motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
    motor.torque_controller = TorqueControlType::voltage;
    motor.controller = MotionControlType::torque;
  	motor.sensor_direction = Direction::CCW;
	motor.zero_electric_angle  = 5.3152;

	motor.PID_velocity.P = 0.1;
	motor.PID_velocity.I = 4;
	motor.PID_velocity.D = 0.000;
	motor.PID_velocity.output_ramp = 1000;
	motor.LPF_velocity.Tf = 0.1;
	motor.PID_velocity.limit = 0.2;

	motor.P_angle.P = 20;
	motor.P_angle.I = 0;
  	motor.P_angle.D = 0.0;
	motor.P_angle.output_ramp = 1000.0;
	motor.P_angle.limit = 10;
	motor.LPF_angle.Tf = 0.01;

	// 初始化电机
	motor.init();
	motor.initFOC();

    motor.PID_velocity.limit = 0.08;

	zero_angle = sensor.getAngle()/PI*180.0f;
    target_angle = zero_angle;
    target_angle = 0;
    err_angle = 0.005;
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
  
       Serial.printf("zero_agle[][1] = %d\n" , target_angle);

       if(target_angle>0 && err_angle<0)
            err_angle = -err_angle;
    }
    else if((now_angle -target_angle) < -gap_angle)
    {
	   
    	digitalWrite(2, HIGH);

        if((target_angle-zero_angle) > min_angle)
            target_angle -= angle_range/count;

        Serial.printf("zero_agle[][1] = %d\n" , target_angle);

        if(target_angle<0 && err_angle>0)
            err_angle = -err_angle;
    }
    else
	    digitalWrite(2, LOW);

    motor.controller = MotionControlType::angle;
    motor.loopFOC();

    motor.move(-(motion.target_angle / 180.0f * PI+err_angle ));
}
void Motion::task_motor(void)
{
	
	while(1)
	{
		// sensor.update();
		
        
        now_angle = sensor.getAngle()/PI*180.0f;
        real_angle  = now_angle - zero_angle;
		vTaskDelay(1);
        // position_check(0,360,36);
        fn31(36,360,2);
        // motor.loopFOC();

        // motor.move(target_angle);
        
    }
  
}

void pulsation(int time , float Amplitude , int16_t count)
{
//   motor.controller = MotionControlType::torque;
//   float row = motor.PID_velocity.limit;
//   motor.PID_velocity.limit = 1;
    int p_time = time;
    while(count--)
    {
        while(p_time--)
        {
            motor.loopFOC();
            motor.move(Amplitude);
            // digitalWrite(2, HIGH);
        }
        p_time = time;
        while(p_time--)
        {
            motor.loopFOC();
            motor.move(-Amplitude);
            // digitalWrite(2, LOW);
        }
        p_time = time;

    }
    motor.move(0);
//   motor.PID_velocity.limit = row;
}
int Motion::fn31(u_int16_t Floors , float Floors_Zone_Du , int Touch_Feedback_Strength)
{
    
    static int last_zhendong = 0; //震动标志位
    int count = Floors_Zone_Du / Floors; //360/12 =30
    if(motion.real_angle%count==0)
    {
    	if(!last_zhendong) 
        {
            last_zhendong = true;
            pulsation((int)5, 0.1, 1);
        }
    }
    else
    {
    	
        last_zhendong = false;
    }        

    motor.loopFOC();
    motor.move(0);
    return 0 ;
}
