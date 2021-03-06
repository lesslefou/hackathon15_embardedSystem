#ifndef ONEM2M_PERSONNAL_API
#define ONEM2M_PERSONNAL_API

enum statOfRequest
{
	OK,
	KO,
};



enum requeteType
{
	GET,
	CREATE,
	UPDATE,
	DELETE
};

enum createType
{
	ACP = 1,
	AE = 2,
	CNT = 3,
	CIN = 4,
	SUB = 23
};

typedef struct json_chaine
{
	char * properti_name;
	void * value;
	int typeOfValue;
	int nbr;
} json_chaine;

struct json_header
{
	int num_validation;
	int RSC;
	int RI;
	int RVI;
	int CL;
	struct json_chaine * body;
};

enum typeOfValue
{
	INT,
	STRING,
	STRING_TAB,
	INT_TAB,
	NODE,
	BOOL
};

#define DEFAULT_CSE_IP "127.0.0.1"
#define DEFAULT_CSE_PORT 8080
#define DEFAULT_USER_AGENT "ACME 0.7.3"
#define DEFAULT_X_M2M_ORIGIN "CAdmin"
#define DEFAULT_X_M2M_ORIGIN_CREATE "CAdmin:CAdmin"
#define DEFAULT_X_M2M_RVI "3"
#define DEFAULT_HTTP_VERSION "HTTP/1.1"
#define CONTENT_TYPE "application/json"
#define DEFAULT_TIME_CONNECTION 5

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include "log_personnal_api.h"

#if defined (WIN32)
    #include <winsock2.h>
    typedef int socklen_t;
#elif defined (linux)
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket(s) close(s)
    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
#endif

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
int connexion(char * IP,int port);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
int closeConnexion(void);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
char * doRequest(char * header,char * body);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
SOCKET my_socket(void);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
SOCKET setMy_Socket(SOCKET soc);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
int driveClock(int time);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
int iAmConnected(void);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
struct json_header jsonToStruct(char * json);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
char * createHeader(char * cible,int type,int create);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
char * tblToJson(json_chaine * data);

#endif
