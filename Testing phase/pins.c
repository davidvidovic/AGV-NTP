#include "pins.h"

// Function implementation
void initGPIO()
{
	// Motor 1
	pinMode(IN1, 			OUTPUT);
	pinMode(IN2, 			OUTPUT);
	pinMode(ENALBE_A, 		PWM_OUTPUT);
	
	// Motor 2
	pinMode(IN3, 			OUTPUT);
	pinMode(IN4, 			OUTPUT);
	pinMode(ENABLE_B, 		PWM_OUTPUT);
	
	// Front HC-SR04
	pinMode(TRIGGER_FRONT, 		OUTPUT);
	pinMode(ECHO_FRONT, 		INPUT);
	
	// Back HC-SR04
	pinMode(TRIGGER_BACK, 		OUTPUT);
	pinMode(ECHO_BACK, 		INPUT);
	
	// ### Add PWM settings
}
