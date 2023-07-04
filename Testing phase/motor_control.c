#include "motor_control.h"

// Functions implementation

// ### To be determined and calibrated

void stop()
{
	// Motor1
	digitalWrite(IN1, 0);
	digitalWrite(IN2, 0);
	
	// Motor 2
	digitalWrite(IN3, 0);
	digitalWrite(IN4, 0);
}

void driveForward()
{
	// Motor1
	digitalWrite(IN1, 1);
	digitalWrite(IN2, 0);
	
	// Motor 2
	digitalWrite(IN3, 1);
	digitalWrite(IN4, 0);
}

void driveBackward()
{
	// Motor1
	digitalWrite(IN1, 0);
	digitalWrite(IN2, 1);
	
	// Motor 2
	digitalWrite(IN3, 0);
	digitalWrite(IN4, 1);
}

void turnLeft()
{
	// Motor1
	digitalWrite(IN1, 1);
	digitalWrite(IN2, 0);
	
	// Motor 2
	digitalWrite(IN3, 0);
	digitalWrite(IN4, 1);
}

void turnRight()
{
	// Motor1
	digitalWrite(IN1, 0);
	digitalWrite(IN2, 1);
	
	// Motor 2
	digitalWrite(IN3, 1);
	digitalWrite(IN4, 0);
}
