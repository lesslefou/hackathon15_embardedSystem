#ifndef GPIO
#define GPIO

enum direction
{
	OUT,
	IN
};

enum value
{
	LOW = '0',
	HIGH = '1'
};

#define GPIO_ROOT "/sys/class/gpio"
#define GPIO_ROOT_ADD "/sys/class/gpio/export"
#define GPIO_ROOT_REMOVE "/sys/class/gpio/unexport"
#include <stdio.h>
#include <stdlib.h>
#define RAPIDE_STP

#include <time.h>

struct gpio
{
	int port;
	FILE * value;
	FILE * direction;
};
typedef struct gpio gpio;

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
FILE * gpio_create_stream_value(int port);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
FILE * gpio_create_stream_direction(int port);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
FILE * fopen_large_time(const char * filename, const char * accessMode);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
gpio gpio_init(int gpio_port);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
int gpio_get_value(gpio my_gpio);

#ifndef RAPIDE_STP
/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
int gpio_direction(gpio in,int direction);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
int gpio_export(int gpio_port);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
int gpio_set_value(gpio my_gpio, int value);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
int gpio_set_direction_in(gpio in);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
int gpio_set_direction_out(gpio in);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
int gpio_destroy(gpio my_gpio);
#else
void gpio_direction(gpio in,int direction);
void gpio_export(int gpio_port);
void gpio_set_value(gpio my_gpio, int value);
void gpio_set_direction_in(gpio in);
void gpio_set_direction_out(gpio in);
void gpio_destroy(gpio my_gpio);
#endif
#endif