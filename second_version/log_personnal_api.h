#ifndef LOG_PERSONNAL_API
#define LOG_PERSONNAL_API

enum achievementType
{
	SUCESS,
	ERROR,
	PROGRESS,
	WARNING,
	INFO
};

#define COLOR_SUCESS "\033[32m"
#define COLOR_ERROR "\033[31m"
#define COLOR_PROGRESS "\033[33m"
#define COLOR_WARNING "\033[35m"
#define COLOR_INFO "\033[34m"
#define COLOR_DEFAULT "\033[0m"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
typedef void PrintString(int,char *,char *);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
void my_log_function(int achievement,char * string,char * function);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
FILE * my_stream_log(FILE * stream);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
PrintString * default_log_function(PrintString printFonction);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
char * giveMeTimeInStr(time_t temps);

#endif
