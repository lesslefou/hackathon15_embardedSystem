#ifndef GPIO
#define GPIO

#define GPIO_ROOT "/sys/class/gpio"
#define GPIO_ROOT_ADD "/sys/class/gpio/export"
#define GPIO_ROOT_REMOVE "/sys/class/gpio/unexport"
#include <stdio.h>
#include <stdlib.h>


#include <time.h>

struct gpio
{
	int port;
	FILE * stream;
};
typedef struct gpio gpio;

gpio gpio_init(int gpio_port,char * direction);
int gpio_direction(int gpio_port,char * direction);
int gpio_export(int gpio_port);
int gpio_set_value(gpio my_gpio, int value);
int gpio_get_value(gpio my_gpio);


int gpio_destroy(gpio my_gpio);
FILE * gpio_create_stream(int gpio_port);
FILE * fopen_large_time(const char * filename, const char * accessMode);

#endif