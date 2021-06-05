#include "gpio.h"

gpio gpio_init(int gpio_port,char * direction)
{
	gpio out;
	out.port = -1;
	if(gpio_export(gpio_port) == 0)
	{
		if(gpio_direction(gpio_port,direction) == 0)
		{
			out.port = gpio_port;
			out.stream = gpio_create_stream(out.port);
		}
	}
	return out;
}

FILE* gpio_create_stream(int gpio_port)
{
	char direct[100];
	sprintf(direct,"%s/gpio%i/value",GPIO_ROOT,gpio_port);
	//printf(direct);
	return fopen_large_time(direct,"w+");
}

FILE * fopen_large_time(const char * filename, const char * accessMode)
{
	FILE * fichier = NULL;
	clock_t begin = clock();
	do
	{
		fichier = fopen(filename, accessMode);
	}
	while(fichier == NULL && begin + CLOCKS_PER_SEC < clock());
	return fichier;
}

int gpio_direction(int gpio_port,char * direction)
{
	char direct[100];
	sprintf(direct,"%s/gpio%i/direction",GPIO_ROOT,gpio_port);
	//printf(direct);
	FILE* fichier = fopen_large_time(direct,"w+");
	if(fichier != NULL)
	{
		fprintf(fichier,"%s",direction);
		fflush(fichier);
		fclose(fichier);
		return 0;
	}
	return 1;
}

int gpio_export(int gpio_port)
{
	FILE* fichier = fopen(GPIO_ROOT_ADD, "w+");
	if(fichier != NULL)
	{
		fprintf(fichier,"%i",gpio_port);
		fflush(fichier);
		fclose(fichier);
		return 0;
	}
	return 1;	
}

int gpio_set_value(gpio my_gpio, int value)
{
	if(my_gpio.stream != NULL)
	{
		fprintf(my_gpio.stream,"%i",value);
		fflush(my_gpio.stream);
	}
	return value;
}

int gpio_destroy(gpio my_gpio)
{
	if(my_gpio.stream != NULL)
	{
		fclose(my_gpio.stream);
	}
	FILE* fichier = fopen(GPIO_ROOT_REMOVE, "w+");
	if(fichier != NULL)
	{
		fprintf(fichier,"%i",my_gpio.port);
		fclose(fichier);
		return 0;
	}
	return 1;
}

int gpio_get_value(gpio my_gpio)
{
	int value = 0;
	if(my_gpio.stream != NULL)
	{
		fseek(my_gpio.stream, 0, SEEK_SET);
		fflush(my_gpio.stream);
		value = fgetc(my_gpio.stream) - '0';
	}
	return value;
}