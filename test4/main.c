#include "log_personnal_api.h"
#include "onem2m_personnal_api.h"
int main(void)
{
	connexion("192.168.43.167",0);
	doRequest(createHeader("/cse-in",GET,0),"");
	/*
	typedef struct json_chaine
	{
		char * properti_name;
		void * value;
		int typeOfValue;
		int nbr;
	} json_chaine;
	*/
	int value1 = 12;
	char * value[3] = {"on","tw","th"};
	json_chaine yo[3] = {{"th","ths",STRING,1},{"hello1",&value1,INT,1},{"hello2",value,STRING_TAB,3}};
	json_chaine all = {"all",yo,NODE,3};
	puts(tblToJson(&all));
	closeConnexion();
}
