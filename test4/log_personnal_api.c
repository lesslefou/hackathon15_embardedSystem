#include "log_personnal_api.h"

void my_log_function(int achievement,char * string,char * function)
{
	char * color = "";
	switch(achievement)
	{
		case SUCESS: color = COLOR_SUCESS; break;
		case ERROR: color = COLOR_ERROR; break;
		case PROGRESS: color = COLOR_PROGRESS; break;
		case WARNING: color = COLOR_WARNING; break;
		case INFO: color = COLOR_INFO; break;
		default: color = COLOR_DEFAULT; break;
	
	}
	char * datetime = giveMeTimeInStr(time(NULL));
	fprintf(my_stream_log(NULL),"\033[37m[%s]\033[36m%s:%s%s%s\n",datetime,function,color,string,COLOR_DEFAULT);
	fflush(my_stream_log(NULL));
}

FILE * my_stream_log(FILE * stream)
{
	static FILE * my_stream;
	if(stream != NULL)
	{
		my_stream = stream;
	}
	else if(my_stream == NULL)
	{
		my_stream = stdout;
	}
	return my_stream;
}

char * giveMeTimeInStr(time_t temps)
{
	static char buffer[80];
	strftime( buffer, 80, "%d/%m/%Y %H:%M:%S", localtime(&temps));
	return buffer;
}

PrintString * default_log_function(PrintString * printFonction)
{
	static PrintString * printeur = NULL;
	if(printFonction != NULL)
	{
		printeur = printFonction;
	}
	else if(printeur == NULL)
	{
		printeur = &my_log_function;
	}
	return printeur;
}
