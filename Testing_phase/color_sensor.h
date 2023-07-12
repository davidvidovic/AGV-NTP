#ifndef COLOR_H
#define	COLOR_H

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define SENSOR_ADDRESS		0x29
#define SENSOR_ID_REG		0x92
#define SENSOR_RGBC_REG		0x81
#define SENSOR_CONTROL_REG	0x8F
#define SENSOR_ENABLE_REG	0x80
#define SENSOR_RED_CH_LOW	0x96
#define SENSOR_RED_CH_HIGH	0x97
#define SENSOR_GREEN_CH_LOW	0x98
#define SENSOR_GREEN_CH_HIGH	0x99
#define SENSOR_BLUE_CH_LOW	0x9A
#define SENSOR_BLUE_CH_HIGH	0x9B

// Funtions declaration
int init_color_sensor();
int read_RED(int fd);
int read_GREEN(int fd);
int read_BLUE(int fd);

#endif
