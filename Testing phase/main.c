#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

// Header files
#include "pins.h"
#include "ultrasonic_sensor.h"
#include "motor_control.h"


int main(int argc, char *argv[])
{
	// Initializing wiringPi library
	if(wiringPiSetup() < 0) 	printf("Error during library initialization.");
	
	// Initializing GPIO pins
	initPins();
	
	// Initially stop the motors
	stop();
	
	while(1)
	{
		
	}
	
	return 0;
}



