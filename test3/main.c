#include <stdio.h>
#include <unistd.h> //sleep();
#include "pi_dht_read.h"
#include <pthread.h>

int main(void)
{
	int tp,hm;
	while(1)
	{
		gpio my_recepeteur = gpio_init(4);
		int value = pi_dht_read(my_recepeteur,&hm,&tp);
		printf("tp: %i,hm: %i and %i\n",tp,hm,value);
		gpio_destroy(my_recepeteur);
		sleep(1);
	}
	return 0;
}