#include "gpio.h"

#define RPI_GPIO_OUT 21
#define RPI_GPIO_IN 4
#include <unistd.h> //sleep();

int main(void)
{
	gpio my_read = gpio_init(RPI_GPIO_IN,"in");
	gpio my_led = gpio_init(RPI_GPIO_OUT,"out");
	printf("%i",gpio_get_value(my_read));
	sleep(1);
	gpio_set_value(my_led, 1);
	sleep(1);
	printf("%i",gpio_get_value(my_read));
	gpio_set_value(my_led, 0);
	sleep(1);
	printf("%i",gpio_get_value(my_read));
	printf("%i",gpio_get_value(my_read));
	printf("%i",gpio_get_value(my_read));
	gpio_destroy(my_read);
	gpio_destroy(my_led);
	return 0;
}

