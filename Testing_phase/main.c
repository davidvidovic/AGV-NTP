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
		// Turn when red is detected
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
//		delay(50);
		//driveForward();
		// Turn when red is detected, keep turning until green is detected, then drive forward until red is detected, and do it in the other direction as well
		
		
		
		
		// Turn when red is detected, keep turning until green is detected, then drive forward until red is detected, but stop if something is in the way
		/*
		while(measure_distance_front() > 10)
		{
			driveForward();
			while(read_RED(fd) < RED_TRESHOLD);	// Possibly add delay
			stop();
			delay(100);
			turnLeft();
			
			while(read_GREEN(fd) < GREEN_TRESHOLD);	// Possibly add delay
			stop();
			delay(100);
			driveForward();
			
			driveForward();
			while(read_RED(fd) < RED_TRESHOLD);	// Possibly add delay
			stop();
			delay(100);
		}
		*/
		
		
		
		// ### NOTE:
		/*
		It will possibly be the easiest to have a bool variable "FORWARD", that will be passed as an argument to all motor_control functions
		Examples:
		drive_forward(1) -> drive_forward();
		drive_forward(0) -> drive_backward();
		
		turnLeft(0) -> turnRight();
		turnLeft(1) -> turnLeft();
		
		// Possibly hard
		*/
		
		
		
		
		/*
		#define L112	1
		#define L107	2
		#define L108	3
		#define L113	4
		#define L109	5
		...
		
		int redLinesCounted = 0;
		int greenLinesCounted = 0;
		int startDestination = L113;
		int finalDestination = L107;
		
		
		
		// 1/3 PART OF TRIP
		
		// wait for the signal to start
		driveForward();
		while(lineCountRED(redLinesCounted) < startDestination)
		{
			while(measure_distance_front() < 20) stop();
			driveForward();
		}
		
		stop();
		redLinesCounted = 0;
		delay(100);
		
		if(startDestination < 111) turnRight();
		else turnLeft();
		
		while(read_GREEN(fd) < GREEN_TRESHOLD)
		{
			while(measure_distance_front() < 20) stop();
			
			// Maybe without this?
			if(startDestination < 111) turnRight();
			else turnLeft();
		}
		
		stop();
		delay(100);
		
		
		if(measure_distance_front() < 300) printf("Door seems to be closed.");
		// wait for the signal and check if distance is > 300 (door is open)
		
		driveForward();
		while(read_RED(fd) < RED_TRESHOLD)
		{
			while(measure_distance_front() < 20) stop();
			driveForward();
		}
		
		stop();
		delay(100);
		
		// wait for signal that delivery is finished
		
		
		
		// 2/3 PART OF TRIP
		
		driveBackward();
		while(read_GREEN(fd) < GREEN_TRESHOLD)
		{
			while(measure_distance_back() < 20) stop();
			driveForward();
		}
		
		stop();
		delay(100);
		
		if(startDestination > finalDestination) turnRight();
		else turnLeft();
		
		while(read_RED(fd) < RED_TRESHOLD)
		{
			while(measure_distance_back() < 20) stop();
			driveForward();
		}
		
		stop();
		delay(100);
		
		driveBackward();
		while(lineCountRED(redLinesCounted) < (startDestination - finalDestination))
		{
			while(measure_distance_back() < 20) stop();
			driveBackward();
		}
		
		stop();
		redLinesCounted = 0;
		delay(100);
		
		// turn towards the door and so on...
		
		
		*/
		
	}
	
	return 0;
}



