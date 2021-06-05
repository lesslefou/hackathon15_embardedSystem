#include <stdio.h>
#include <unistd.h> //sleep();
#include "pi_dht_read.h"

int main(void)
{
	float tp,hm;
	while(1)
	{
		int value = pi_dht_read(DHT11,4,&tp,&hm);
		printf("tp: %f,hm: %f and %i\n",tp,hm,value);
		sleep(2);
	}
	return 0;
}