#include "log_personnal_api.h"
#include "onem2m_personnal_api.h"
#include "gpio.h"
#include "pi_dht_read.h"
#include <pthread.h>


int hm,tp = -1;
int dontStopMeSow = 0;

void * end(void * arg)
{
	dontStopMeSow = getc(stdin);
	return NULL;
}

void * getCapteur(void * arg)
{
	gpio my_recepeteur;
	gpio my_led[3];
	my_led[0] = gpio_init(17);
	my_led[1] = gpio_init(27);
	my_led[2] = gpio_init(22);
	gpio_set_direction_out(my_led[0]);
	gpio_set_direction_out(my_led[1]);
	gpio_set_direction_out(my_led[2]);
	gpio_set_value(my_led[0],HIGH);
	my_recepeteur = gpio_init(4);
	while(dontStopMeSow == 0)
	{
		time_t begin = time(NULL) + 2;
		int tphm,tptp;
		int error = pi_dht_read(my_recepeteur,&tphm,&tptp);
		if(error == 0)
		{
			hm = tphm;
			tp = tptp;
			gpio_set_value(my_led[1],HIGH);
		}
		else
		{
			gpio_set_value(my_led[1],LOW);
		}
		while(dontStopMeSow == 0 && begin > time(NULL));
	}
	gpio_destroy(my_recepeteur);
	gpio_destroy(my_led[0]);gpio_destroy(my_led[1]);gpio_destroy(my_led[2]);
	return NULL;
}

void * sendValue(void * arg)
{
	/*connexion("192.168.43.167",0);
	char * message = doRequest(createHeader("/cse-in/forestDrone/droneData/environement",GET,0),"");
	int number = 0;
	if(message != null)
		sscanf(message,"%*s %i",&number);
	if(number == 404)
	{
		char * recherche[1] = {"Type/drone"};
		char * version[1] = {"RELASE_ALPHA"};
		int val = false;
		json_chaine value[5] = {{"lbl",recherche,STRING_TAB,1},{"api","NforestDrone",STRING,1},{"rr",&val,BOOL,1},{"rn","forestDrone",STRING,1},{"srv",version,STRING_TAB,1}};
		json_chaine all = {"m2m:ae",value,NODE,5};
		printf("%s",tblToJson(&all));
		fflush(stdout);
		connexion("192.168.43.167",0);
		message = doRequest(createHeader("/cse-in",CREATE,AE),tblToJson(&all));
		
		json_chaine value1[1] = {{"rn","droneData",STRING,1}};
		json_chaine all1 = {"m2m:cnt",value1,NODE,1};
		connexion("192.168.43.167",0);
		message = doRequest(createHeader("/cse-in/forestDrone",CREATE,CNT),tblToJson(&all1));
		
		json_chaine value2[4] = {{"rn","environement",STRING,1},{"tp",&tp,INT,1},{"hm",&hm,INT,1},{"ri","fodaen",STRING,1}};
		json_chaine all2 = {"m2m:env",value2,NODE,4};
		connexion("192.168.43.167",0);
		message = doRequest(createHeader("/cse-in/forestDrone/droneData",CREATE,13),tblToJson(&all2));
	}*/
	while(dontStopMeSow == 0)
	{
		time_t begin = time(NULL) + 10;
		connexion("192.168.43.167",0);
		json_chaine value[2] = {{"tp",&tp,INT,1},{"hm",&hm,INT,1}};
		json_chaine all = {"m2m:env",value,NODE,2};
		doRequest(createHeader("/cse-in/forestDrone/droneData/environement",UPDATE,CIN),tblToJson(&all));
		while(dontStopMeSow == 0 && begin > time(NULL));
	}
	closeConnexion();
	return NULL;
}

int main(void)
{
	pthread_t thread[4];
	pthread_create(&thread[0], NULL,end, NULL);
	pthread_create(&thread[1], NULL,getCapteur, NULL);
	pthread_create(&thread[2], NULL,sendValue, NULL);
	
	pthread_join(thread[0],NULL);
	pthread_join(thread[1],NULL);
	pthread_join(thread[2],NULL);

	return 0;
}
