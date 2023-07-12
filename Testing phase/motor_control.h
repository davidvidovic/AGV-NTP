#ifndef MOTOR_H
#define	MOTOR_H

#include <wiringPi.h>
#include "pins.h"

// Funtions declaration
void stop();
void driveForward();
void driveBackward();
void turnLeft();
void turnRight();
void backward_turnLeft();
void backward_turnRight();

#endif
