#include "onem2m_personnal_api.h"

int connexion(char * IP,int port)
{
	if(iAmConnected())
	{
		closeConnexion();
		my_log_function(WARNING,"déconnection automatique dû à une nouvelle connexion","connexion");
	}

	char * addr; 
	int wall; 
	char bufferLog[100];
	if(IP == NULL)
	{
		addr = DEFAULT_CSE_IP;
		sprintf(bufferLog,"Default adresse %s is used",addr);
	}
	else
	{
		addr = IP;
		sprintf(bufferLog,"Set adresse %s",addr);
	}
	my_log_function(INFO,bufferLog,"connexion");
	
	if(port == 0)
	{
		wall = DEFAULT_CSE_PORT;
		sprintf(bufferLog,"Default port %d is used",wall);
	}
	else
	{
		wall = port;
		sprintf(bufferLog,"Set adresse %d",wall);
	}	
	my_log_function(INFO,bufferLog,"connexion");
	
	#if defined (WIN32)
	WSADATA WSAData;
	int erreur = WSAStartup(MAKEWORD(2,2), &WSAData);
	#else
	int erreur = 0;
	#endif

	SOCKET sock;
	SOCKADDR_IN sin;
	if(!erreur)
	{
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if(sock != -1)
		{
			sin.sin_addr.s_addr = inet_addr(addr);
			sin.sin_family = AF_INET;
			sin.sin_port = htons(wall);
			sprintf(bufferLog,"Tentative de connexion à %s sur le port %d", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
			my_log_function(INFO,bufferLog,"connexion");
			driveClock(1);
			int testError = SOCKET_ERROR;
			do
			{
				fcntl(sock,O_NONBLOCK);
				testError = connect(sock, (SOCKADDR*)&sin, sizeof(sin));
			}
			while(testError == -1 && driveClock(-1));
			if(testError != SOCKET_ERROR)
			{
				sprintf(bufferLog,"Connexion à %s sur le port %d", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
				my_log_function(SUCESS,bufferLog,"connexion");
				fcntl(sock,O_NONBLOCK);
				setMy_Socket(sock);
				return 1;
			}
			sprintf(bufferLog,"Connexion échouer à %s sur le port %d", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
			my_log_function(ERROR,bufferLog,"connexion");
			return 2;
		}
	}
	my_log_function(ERROR,"Impossible de se connecter","connexion");
	return 3;
}

int driveClock(int tim)
{
	static time_t begin = 0;
	if(tim > 0)
	{
		begin = time(NULL) + tim;
		my_log_function(INFO,"Début de la clock","driveClock");
	}
	else if(begin < time(NULL))
	{
		my_log_function(INFO,"Fin de la clock","driveClock");
		return 0;
	}
	return 1;
}


int closeConnexion(void)
{
	if(iAmConnected())
	{
		closesocket(my_socket());
		#if defined (WIN32)
		WSACleanup();
		#endif
		my_log_function(INFO,"Déconnection","closeConnexion");
		return 1;
	}
	my_log_function(WARNING,"Aucune connexion existante","closeConnexion");
	return 4;
}

char * doRequest(char * header,char * body)
{
	static char server_reply[2000];

	sprintf(server_reply,"%s%i\r\n\r\n{%s}\r\n\r\n",header,strlen(body)+4,body);
	free(body);
	my_log_function(INFO,server_reply,"doRequest");
	if (send(my_socket(), server_reply, strlen(server_reply),MSG_DONTWAIT) < 0)
	{
		my_log_function(ERROR,"Send fail","doRequest");
	}
	else
	{
		my_log_function(SUCESS,"Send success","doRequest");
	}
	sleep(1);
	memset( server_reply, 0, 2000 );
	if (recv(my_socket(), server_reply, 2000, MSG_DONTWAIT) > 0)
	{
		my_log_function(SUCESS,"reception success:","doRequest");
		my_log_function(INFO,server_reply,"doRequest");
	} 
	else
	{
		my_log_function(WARNING,"we have nothing","doRequest");
		return NULL;
	}
	return server_reply;
}

char * createHeader(char * cible,int type,int create)
{
	static char message[2000];
	char * Type;
	char Create[5];
	Create[0] = '\0';
	static int requestNumber = 0;
	char origine[100] = DEFAULT_X_M2M_ORIGIN;
	switch(type)
	{
		case GET: Type = "GET"; break;
		case CREATE: Type = "POST"; sprintf(Create,"%i",create); strcpy(origine,DEFAULT_X_M2M_ORIGIN_CREATE); break;
		case UPDATE: Type = "PUT"; break;
		case DELETE: Type = "DELETE"; break;
		default: Type = "GET"; break;
	}
	
	sprintf(message,
"%s %s %s\r\n\
User-Agent: %s\r\n\
Content-Type: %s;ty=%s\r\n\
Accept: %s\r\n\
X-M2M-Origin: %s\r\n\
X-M2M-RI: %i\r\n\
X-M2M-RVI: %s\r\n\
Content-Length:",Type,cible,DEFAULT_HTTP_VERSION,DEFAULT_USER_AGENT,CONTENT_TYPE,Create,CONTENT_TYPE,origine,requestNumber,DEFAULT_X_M2M_RVI);
	 requestNumber++;
	
	return message;
}

int iAmConnected(void)
{
	return my_socket() > 0;
}

SOCKET my_socket(void)
{
	return setMy_Socket(-1);
}

SOCKET setMy_Socket(SOCKET soc)
{
	static SOCKET socket = 0;
	if(soc >= 0)
	{
		socket = soc;
	}
	return socket;
}

char * tblToJson(json_chaine * data)
{
	char * retour = malloc(sizeof(char)*2000);
	retour[0] = '\0';
	strcat(retour,"\"");
	strcat(retour,data->properti_name);
	strcat(retour,"\":");
	char yo[10];
	switch(data->typeOfValue)
	{
		case INT : 
			sprintf(yo,"%i",*(int*)data->value);
			strcat(retour,yo);
			break;
		case BOOL : 
			switch(*(int*)data->value)
			{
				case false : strcat(retour,"false"); break;
				case true : strcat(retour,"true"); break;
			}
			break;
		case STRING : 
			strcat(retour,"\"");
			strcat(retour,(char*)data->value);
			strcat(retour,"\"");
			break;
		case STRING_TAB : 
			strcat(retour,"[");
			for(int i = 0; i < data->nbr; i++)
			{
				strcat(retour,"\"");
				char * * other = (char**)data->value;
				strcat(retour,other[i]);
				strcat(retour,"\"");
				if(i < data->nbr - 1)
					strcat(retour,",");
			}
			strcat(retour,"]");
			break;
		case INT_TAB : 
			strcat(retour,"[");
			for(int i = 0; i < data->nbr; i++)
			{
				int * tb = (int*)data->value;
				sprintf(yo,"%i",tb[i]);
				strcat(retour,yo);
				if(i < data->nbr - 1)
					strcat(retour,",");
			}
			strcat(retour,"]");
			break;
		case NODE : 
			strcat(retour,"{");
			for(int i = 0; i < data->nbr; i++)
			{
				json_chaine * other = (json_chaine *) data->value;
				char * value = tblToJson(&other[i]);
				strcat(retour,value);
				if(i < data->nbr - 1)
					strcat(retour,",");
				free(value);
			}
			strcat(retour,"}");
			break;
		default: break;
	}
	return retour;
}
