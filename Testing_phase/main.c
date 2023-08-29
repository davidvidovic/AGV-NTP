#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

// Header files
#include "pins.h"
#include "ultrasonic_sensor.h"
#include "motor_control.h"
#include "color_sensor.h"

// Defining tresholds for each color
#define RED_TRESHOLD	620
#define GREEN_TRESHOLD	1300
#define BLUE_TRESHOLD	500

int main(int argc, char *argv[])
{
	printf("Starting initialization...\n");

	// Initializing wiringPi library
	if(wiringPiSetup() < 0) 	printf("Error during library initialization.");
	
	// Initializing GPIO pins
	initGPIO();
	
	// Initializing PWM settings
	initPWM();
	
	// Initially PWM is 0
	// No need to explicitly call this function again, it will be called whenever the motor_control::stop() is called
	PWM_OFF();
	
	// Initializing I2C communication and TCS3472 color sensor
	int fd;
	//fd = init_color_sensor();
	
	// Initially stop the motors
	stop();
	
	int maxred = 0;
	int maxgreen = 0;
	int maxblue = 0;
	int maxclear = 0;

	int red = 0;
	int green = 0;
	int blue = 0;
	int clear = 0;

	printf("Driving...\n");
	driveForward();

	while(1)
	{
		//driveBackward();
		
		
	/*
		red = read_RED(fd);
		green = read_GREEN(fd);
		blue = read_BLUE(fd);
		clear = read_CLEAR(fd);

		if(red > maxred)
		{
			maxred = red;
			printf("+R=%d  G=%d  B=%d  C=%d\n", read_RED(fd), read_GREEN(fd), read_BLUE(fd), read_CLEAR(fd));
		}
		else if(green > maxgreen)
		{
			maxgreen = green;
			printf("R=%d  +G=%d  B=%d  C=%d\n", read_RED(fd), read_GREEN(fd), read_BLUE(fd), read_CLEAR(fd));
		}
		else if(blue > maxblue)
		{
			maxblue = blue;
			printf("R=%d  G=%d  +B=%d  C=%d\n", read_RED(fd), read_GREEN(fd), read_BLUE(fd), read_CLEAR(fd));
		}
		else if(clear > maxclear)
		{
			maxclear = clear;
			printf("R=%d  G=%d  B=%d  +C=%d\n", read_RED(fd), read_GREEN(fd), read_BLUE(fd), read_CLEAR(fd));
		}
*/
	
	}
	
	return 0;
}



