#include "gpio.h"

FILE * gpio_create_stream_direction(int port)
{
	char direct[100];
	sprintf(direct,"%s/gpio%i/direction",GPIO_ROOT,port);
	return fopen_large_time(direct,"w+");
}

FILE* gpio_create_stream_value(int port)
{
	char direct[100];
	sprintf(direct,"%s/gpio%i/value",GPIO_ROOT,port);
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
	while(fichier == NULL && begin + CLOCKS_PER_SEC > clock());
	return fichier;
}

#ifndef RAPIDE_STP
gpio gpio_init(int gpio_port)
{
	gpio out = {-1,NULL,NULL};
	if(gpio_export(gpio_port) == 0)
	{
		out.port = gpio_port;
		out.direction = gpio_create_stream_direction(out.port);
		out.value = gpio_create_stream_value(out.port);
	}
	return out;
}

int gpio_direction(gpio in,int direction)
{
	if(in.direction != NULL)
	{
		switch(direction)
		{
			case OUT : fputs("out",in.direction); break;
			case IN : fputs("in",in.direction); break;
			default : break;
		}
		fflush(in.direction);
		return direction;
	}
	return -1;
}

int gpio_set_direction_in(gpio in)
{
	if(in.direction != NULL)
	{
		fputs("in",in.direction);
		fflush(in.direction);
		return 1;
	}
	return 0;
}

int gpio_set_direction_out(gpio in)
{
	if(in.direction != NULL)
	{
		fputs("out",in.direction);
		fflush(in.direction);
		return 1;
	}
	return 0;
}

int gpio_export(int gpio_port)
{
	FILE* fichier = fopen(GPIO_ROOT_ADD, "w+");
	if(fichier != NULL)
	{
		fprintf(fichier,"%i",gpio_port);
		fclose(fichier);
		return 0;
	}
	return 1;	
}

int gpio_set_value(gpio my_gpio, int value)
{
	if(my_gpio.value != NULL)
	{
		fputc(value,my_gpio.value);
		fflush(my_gpio.value);
	}
	return value;
}

int gpio_destroy(gpio my_gpio)
{
	if(my_gpio.value != NULL)
	{
		fclose(my_gpio.value);
	}
	if(my_gpio.direction != NULL)
	{
		fclose(my_gpio.direction);
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
	if(my_gpio.value != NULL)
	{
		fseek(my_gpio.value, 0, SEEK_SET);
		fflush(my_gpio.value);
		return fgetc(my_gpio.value) - '0';
	}
	return -1;
}
#else
gpio gpio_init(int gpio_port)
{
	gpio_export(gpio_port);
	return (gpio){gpio_port,gpio_create_stream_value(gpio_port),gpio_create_stream_direction(gpio_port)};
}

void gpio_direction(gpio in,int direction)
{
	switch(direction)
	{
		case OUT : fputs("out",in.direction); fflush(in.direction); break;
		case IN : fputs("in",in.direction); fflush(in.direction); break;
		default : break;
	}
}

void gpio_set_direction_in(gpio in)
{
	fputs("in",in.direction);
	fflush(in.direction);
}

void gpio_set_direction_out(gpio in)
{
	fputs("out",in.direction);
	fflush(in.direction);
}

void gpio_export(int gpio_port)
{
	FILE* fichier = fopen(GPIO_ROOT_ADD, "w+");
	fprintf(fichier,"%i",gpio_port);
	fclose(fichier);
}

void gpio_set_value(gpio my_gpio, int value)
{
	fputc(value,my_gpio.value);
	fflush(my_gpio.value);
}

void gpio_destroy(gpio my_gpio)
{
	fclose(my_gpio.value);
	fclose(my_gpio.direction);
	FILE* fichier = fopen(GPIO_ROOT_REMOVE, "w+");
	fprintf(fichier,"%i",my_gpio.port);
	fclose(fichier);
}

int gpio_get_value(gpio my_gpio)
{
	fseek(my_gpio.value, 0, SEEK_SET);
	fflush(my_gpio.value);
	return fgetc(my_gpio.value) - '0';
}


#endif