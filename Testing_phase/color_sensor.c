#include "color_sensor.h"

// Delete if not neccessary
#include <wiringPi.h>
#include <wiringPiI2C.h>

// Functions implementation
int init_color_sensor()
{
	printf("Color sensor initialization...\n");

	// I2C initialization
	int fd;
	fd = wiringPiI2CSetup(SENSOR_ADDRESS);
	if(fd == -1){
		printf("Error during I2C initialization.");
		return -1;
	}
	
	// Check if sensor is available and communicates by checking ID register
	int ID;
	wiringPiI2CWrite(fd, SENSOR_ID_REG);
	ID = wiringPiI2CRead(fd);
	
	// 77 = 0x4D
	// 68 = 0x44
	if(ID != 77 && ID != 68){
		printf("Error! Sensor not found.");
		return -1;
	}
	else	printf("Sensor found! Sensor ID is %d.\n", ID);
	
	// Set RGBC time to a near maximum sample rate
	wiringPiI2CWriteReg8(fd, SENSOR_RGBC_REG, 0xC0);
	
	// Set minimum gain in control register - gain is x1
	// If needed better resolution, change to x4 or x16
	wiringPiI2CWriteReg8(fd, SENSOR_CONTROL_REG, 0x16);
	
	// Enable register - set bits to start ADC and clock
	wiringPiI2CWriteReg8(fd, SENSOR_ENABLE_REG, 0x03);
	
	return fd;
}

int read_CLEAR(int fd)
{
	int clear_low_byte;
	int clear_high_byte;
	
	// Reading low byte
	wiringPiI2CWrite(fd, SENSOR_CLEAR_CH_LOW);
	clear_low_byte = wiringPiI2CRead(fd);
	
	// Reading high byte
	wiringPiI2CWrite(fd, SENSOR_CLEAR_CH_HIGH);
	clear_high_byte = wiringPiI2CRead(fd);
	
	return (((int)clear_high_byte << 8) | clear_low_byte);
}

int read_RED(int fd)
{
	int red_low_byte;
	int red_high_byte;
	
	// Reading low byte
	wiringPiI2CWrite(fd, SENSOR_RED_CH_LOW);
	red_low_byte = wiringPiI2CRead(fd);
	
	// Reading high byte
	wiringPiI2CWrite(fd, SENSOR_RED_CH_HIGH);
	red_high_byte = wiringPiI2CRead(fd);
	
	return (((int)red_high_byte << 8) | red_low_byte);
}

int read_GREEN(int fd)
{
	int green_low_byte;
	int green_high_byte;
	
	// Reading low byte
	wiringPiI2CWrite(fd, SENSOR_GREEN_CH_LOW);
	green_low_byte = wiringPiI2CRead(fd);
	
	// Reading high byte
	wiringPiI2CWrite(fd, SENSOR_GREEN_CH_HIGH);
	green_high_byte = wiringPiI2CRead(fd);
	
	return (((int)green_high_byte << 8) | green_low_byte);
}

int read_BLUE(int fd)
{
	int blue_low_byte;
	int blue_high_byte;
	
	// Reading low byte
	wiringPiI2CWrite(fd, SENSOR_BLUE_CH_LOW);
	blue_low_byte = wiringPiI2CRead(fd);
	
	// Reading high byte
	wiringPiI2CWrite(fd, SENSOR_BLUE_CH_HIGH);
	blue_high_byte = wiringPiI2CRead(fd);
	
	return (((int)blue_high_byte << 8) | blue_low_byte);
}
