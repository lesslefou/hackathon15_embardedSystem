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

FILE * gpio_create_stream_value(int port);
FILE * gpio_create_stream_direction(int port);
FILE * fopen_large_time(const char * filename, const char * accessMode);

gpio gpio_init(int gpio_port);

int gpio_get_value(gpio my_gpio);

#ifndef RAPIDE_STP
int gpio_direction(gpio in,int direction);
int gpio_export(int gpio_port);
int gpio_set_value(gpio my_gpio, int value);
int gpio_set_direction_in(gpio in);
int gpio_set_direction_out(gpio in);
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