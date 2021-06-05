#include "log_personnal_api.h"
#include "onem2m_personnal_api.h"
#include "gpio.h"
#include "pi_dht_read.h"
#include "serialGPS.h"
#include <pthread.h>

int hm,tp = -1;
coord lat,lon;
int dontStopMeSow = 0;
int canIsendThis = 0;

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
	while(connexion("192.168.43.167",0) != 1 && dontStopMeSow == 0);
	char * recherche[1] = {"Type/drone"};
	char * version[1] = {"RELASE_ALPHA"};
	int val = false;
	json_chaine value[5] = {{"lbl",recherche,STRING_TAB,1},{"api","NforestDrone",STRING,1},{"rr",&val,BOOL,1},{"rn","forestDrone",STRING,1},{"srv",version,STRING_TAB,1}};
	json_chaine all = {"m2m:ae",value,NODE,5};
	printf("%s",tblToJson(&all));
	fflush(stdout);
	while(connexion("192.168.43.167",0) != 1 && dontStopMeSow == 0);
	doRequest(createHeader("/cse-in",CREATE,AE),tblToJson(&all));
	
	json_chaine value1[1] = {{"rn","droneData",STRING,1}};
	json_chaine all1 = {"m2m:cnt",value1,NODE,1};
	while(connexion("192.168.43.167",0) != 1 && dontStopMeSow == 0);
	doRequest(createHeader("/cse-in/forestDrone",CREATE,CNT),tblToJson(&all1));
	
	json_chaine value2[4] = {{"rn","environement",STRING,1},{"tp",&tp,INT,1},{"hm",&hm,INT,1},{"ri","fodaen",STRING,1}};
	json_chaine all2 = {"m2m:env",value2,NODE,4};
	while(connexion("192.168.43.167",0) != 1 && dontStopMeSow == 0);
	doRequest(createHeader("/cse-in/forestDrone/droneData",CREATE,13),tblToJson(&all2));
	
	json_chaine forLat[5] = {{"degree",&lat.d,INT,1},{"minute",&lat.m,INT,1},{"second",&lat.s,INT,1},{"msecond",&lat.ms,INT,1},{"direction",&lat.D,INT,1}};
	json_chaine forLon[5] = {{"degree",&lon.d,INT,1},{"minute",&lon.m,INT,1},{"second",&lon.s,INT,1},{"msecond",&lon.ms,INT,1},{"direction",&lon.D,INT,1}};
	json_chaine value3[4] = {{"rn","position",STRING,1},{"latitude",forLat,NODE,5},{"longitude",forLon,NODE,5},{"ri","fodapo",STRING,1}};
	json_chaine all3 = {"m2m:pos",value3,NODE,4};
	while(connexion("192.168.43.167",0) != 1 && dontStopMeSow == 0);
	doRequest(createHeader("/cse-in/forestDrone/droneData",CREATE,13),tblToJson(&all3));
	
	while(dontStopMeSow == 0)
	{
		time_t begin = time(NULL) + 10;
		while(connexion("192.168.43.167",0) != 1 && dontStopMeSow == 0);
		json_chaine value4[2] = {{"tp",&tp,INT,1},{"hm",&hm,INT,1}};
		json_chaine all4 = {"m2m:env",value4,NODE,2};
		doRequest(createHeader("/cse-in/forestDrone/droneData/environement",UPDATE,CIN),tblToJson(&all4));
		closeConnexion();
		if(canIsendThis == 1)
		{
			while(connexion("192.168.43.167",0) != 1 && dontStopMeSow == 0);
			json_chaine forLat[5] = {{"degree",&lat.d,INT,1},{"minute",&lat.m,INT,1},{"second",&lat.s,INT,1},{"msecond",&lat.ms,INT,1},{"direction",&lat.D,INT,1}};
			json_chaine forLon[5] = {{"degree",&lon.d,INT,1},{"minute",&lon.m,INT,1},{"second",&lon.s,INT,1},{"msecond",&lon.ms,INT,1},{"direction",&lon.D,INT,1}};
			json_chaine value5[4] = {{"latitude",forLat,NODE,5},{"longitude",forLon,NODE,5}};
			json_chaine all5 = {"m2m:pos",value5,NODE,2};
			doRequest(createHeader("/cse-in/forestDrone/droneData/position",UPDATE,CIN),tblToJson(&all5));
			closeConnexion();
		}
		while(dontStopMeSow == 0 && begin > time(NULL));
	}
	closeConnexion();
	return NULL;
}

void * getMyGPS(void * arg)
{
	if(!initGPS("/dev/ttyS0"))
		return NULL;
	sendMessageToGPS(PMTK_SET_NMEA_OUTPUT_GGAONLY);
	sendMessageToGPS(PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ);
	while(dontStopMeSow == 0)
	{
		time_t begin = time(NULL) + 10;
		char buffer[255];
		if(convertSerialGpsToTwoCoord(getMessageFromGPS(buffer,255),&lat,&lon))
			canIsendThis = 1;
		while(dontStopMeSow == 0 && begin > time(NULL));
	}
	closeGPS();
	return NULL;
}


int main(void)
{
	lat.d = -1;
	lon.d = -1;
	pthread_t thread[4];
	pthread_create(&thread[0], NULL,end, NULL);
	pthread_create(&thread[1], NULL,getCapteur, NULL);
	pthread_create(&thread[2], NULL,sendValue, NULL);
	pthread_create(&thread[3], NULL,getMyGPS, NULL);
	
	pthread_join(thread[0],NULL);
	pthread_join(thread[1],NULL);
	pthread_join(thread[2],NULL);
	pthread_join(thread[3],NULL);
	
	return 0;
}
