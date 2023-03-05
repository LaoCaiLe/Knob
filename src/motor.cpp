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
    motor.controller = MotionControlType::angle;
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
    target = zero_angle;
}
static void position_check()
{
    static float gap_angle = 5;
    motion.now_angle = sensor.getAngle()/PI*180.0f;
 

    if((motion.now_angle -motion.target) > gap_angle)
    {

    	digitalWrite(2, HIGH);
        if((motion.target-motion.zero_angle) < 100)
            motion.target += 10;
        Serial.printf("circle_coord[][0] = %.3f\n" , motion.target);
        // if(motion.target >6.28)
        //     motion.target -= 6.28;
        // pulsation(8000,0.5,1);
       
    }
    else if((motion.now_angle -motion.target) < -gap_angle)
    {
	   
    	digitalWrite(2, HIGH);

        if((motion.target-motion.zero_angle) > -100)
            motion.target -= 10;
        Serial.printf("circle_coord[][0] = %.3f\n" , motion.target);
        // if(motion.target >6.28)
        //     motion.target -= 6.28;

        // pulsation(8000,0.5,2);
    }
    else
	    digitalWrite(2, LOW);

    //  motor.controller = MotionControlType::angle;

}
void Motion::task_motor(void)
{
	
	while(1)
	{
		// sensor.update();
		
		motor.loopFOC();
        // if(motion.target>0)
            motor.move(-(motion.target / 180.0f * PI+err_angle ));
        // else
            // motor.move(-(motion.target / 180 * PI - 0.05));
        // motor.move(-(motion.target));
		vTaskDelay(2);
        position_check();
    }
  
}

void pulsation(int16_t time , float Amplitude , int16_t count)
{
  motor.controller = MotionControlType::torque;
  float row = motor.PID_velocity.limit;
  motor.PID_velocity.limit = 1;
  while(count--)
  {
    motor.move(Amplitude);
    delayMicroseconds(time);
    motor.move(-Amplitude);
    delayMicroseconds(time);
  }
  motor.move(0);
  motor.PID_velocity.limit = row;
}
