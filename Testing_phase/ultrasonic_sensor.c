#include "ultrasonic_sensor.h"

// Functions implementation
float measure_distance_front()
{
	int start_time;
	int finish_time;
	
	// Making sure TRIGGER pin is LOW
	digitalWrite(TRIGGER_FRONT, 0);
	delayMicroseconds(10);
	
	// Sending 10us HIGH pulse
	digitalWrite(TRIGGER_FRONT, 1);
	delayMicroseconds(10);
	digitalWrite(TRIGGER_FRONT, 0);
	
	// Measuring distance starts as the ECHO pin becomes HIGH
	while(digitalRead(ECHO_FRONT == 0)) start_time = micros();
	
	// While ECHO pin is HIGH, ultrasonic waves have not returned to the reciever
	// As the waves return, ECHO pin becames LOW
	while(digitalRead(ECHO_FRONT == 1)) finish_time = micros();
	
	// Calculating distance based on time elapsed and known speed of sound in the air
	return (float)(finish_time - start_time) * 34300.0 / 2;
}

float measure_distance_back()
{
	int start_time;
	int finish_time;
	
	digitalWrite(TRIGGER_BACK, 0);
	delayMicroseconds(10);
	
	digitalWrite(TRIGGER_BACK, 1);
	delayMicroseconds(10);
	digitalWrite(TRIGGER_BACK, 0);
	
	while(digitalRead(ECHO_BACK == 0)) start_time = micros();
	
	while(digitalRead(ECHO_BACK == 1)) finish_time = micros();

	return (float)(finish_time - start_time) * 34300.0 / 2;
}
