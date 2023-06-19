#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

// Header files
#include "pins.h"
#include "ultrasonic_sensor.h"
#include "motor_control.h"
#include "color_sensor.h"


int main(int argc, char *argv[])
{
	// Initializing wiringPi library
	if(wiringPiSetup() < 0) 	printf("Error during library initialization.");
	
	// Initializing GPIO pins
	initGPIO();
	
	// Initializing I2C communication and TCS3472 color sensor
	int fd;
	fd = init_color_sensor();
	
	// Initially stop the motors
	stop();
	
	while(1)
	{
		// Testing only!
		printf("R: %d G: %d B: %d\n\n", read_RED(fd), read_GREEN(fd), read_BLUE(fd));
		delay(1000);
	}
	
	return 0;
}



