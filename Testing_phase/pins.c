#include "pins.h"

// Function implementation
void initGPIO()
{
	// Motor 1
	pinMode(IN1, 		OUTPUT);
	pinMode(IN2, 		OUTPUT);
	pinMode(ENABLE_A, 	PWM_OUTPUT);
	
	// Motor 2
	pinMode(IN3, 		OUTPUT);
	pinMode(IN4, 		OUTPUT);
	pinMode(ENABLE_B, 	PWM_OUTPUT);
	
	// Front HC-SR04
	pinMode(TRIGGER_FRONT, 	OUTPUT);
	pinMode(ECHO_FRONT, 	INPUT);
	
	// Back HC-SR04
	pinMode(TRIGGER_BACK, 	OUTPUT);
	pinMode(ECHO_BACK, 	INPUT);
}

void initPWM()
{
	pwmSetClock(PWM_CLOCK);
	pwmSetRange(PWM_RANGE);
	pwmSetMode(PWM_MODE_MS);
}

void PWM_ON(int duty_cycle1, int duty_cycle2)
{
	pwmWrite(ENABLE_A, duty_cycle1);
	pwmWrite(ENABLE_B, duty_cycle2);
}

void PWM_OFF()
{
	pwmWrite(ENABLE_A, 0);
	pwmWrite(ENABLE_B, 0);
}
