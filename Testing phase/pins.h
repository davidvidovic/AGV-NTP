#ifndef PINS_H
#define	PINS_H

#include <wiringPi.h>

// Macros used to indicate which GPIO pins are connected to L298 pins
// Motor 1 macros
#define IN1				5
#define IN2				4
#define ENABLE_A			10

// Motor 2 macros
#define IN3				11
#define IN4				31
#define ENABLE_B			15

// Macros used to indicate which GPIO pins are connected to HC-SR04 pins
// Front HC-SR04
#define ECHO_FRONT			28
#define TRIGGER_FRONT			27

// Back HC-SR04
#define ECHO_FRONT			7
#define TRIGGER_FRONT			25


// Funtions declaration

// Function to init GPIO pins as inputs, outputs or PWM outputs
// To be called in setup part of the program
void initGPIO();

#endif
