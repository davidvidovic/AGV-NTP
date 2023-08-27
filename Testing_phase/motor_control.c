#include "motor_control.h"

// Functions implementation

// ### To be determined and calibrated

void stop()
{
	PWM_OFF();
	
	// Motor1
	digitalWrite(IN1, 0);
	digitalWrite(IN2, 0);
	
	// Motor 2
	digitalWrite(IN3, 0);
	digitalWrite(IN4, 0);
}

void driveForward()
{
	PWM_ON(100, 100);
	
	// Motor1
	digitalWrite(IN1, 1);
	digitalWrite(IN2, 0);
	
	// Motor 2
	digitalWrite(IN3, 0);
	digitalWrite(IN4, 1);
}

void driveBackward()
{
	PWM_ON(50, 50);
	
	// Motor1
	digitalWrite(IN1, 0);
	digitalWrite(IN2, 1);
	
	// Motor 2
	digitalWrite(IN3, 1);
	digitalWrite(IN4, 0);
}

void turnLeft()
{
	PWM_ON(EN_A_VALUE, EN_B_VALUE);
	
	// Motor1
	digitalWrite(IN1, 1);
	digitalWrite(IN2, 0);
	
	// Motor 2
	digitalWrite(IN3, 0);
	digitalWrite(IN4, 0);
}

void turnRight()
{
	PWM_ON(EN_A_VALUE, EN_B_VALUE);
	
	// Motor1
	digitalWrite(IN1, 0);
	digitalWrite(IN2, 0);
	
	// Motor 2
	digitalWrite(IN3, 0);
	digitalWrite(IN4, 1);
}

void backward_turnLeft()
{
	PWM_ON(EN_A_VALUE, EN_B_VALUE);
	
	// Motor1
	digitalWrite(IN1, 0);
	digitalWrite(IN2, 0);
	
	// Motor 2
	digitalWrite(IN3, 1);
	digitalWrite(IN4, 0);
}

void backward_turnRight()
{
	PWM_ON(EN_A_VALUE, EN_B_VALUE);
	
	// Motor1
	digitalWrite(IN1, 0);
	digitalWrite(IN2, 1);
	
	// Motor 2
	digitalWrite(IN3, 0);
	digitalWrite(IN4, 0);
}
