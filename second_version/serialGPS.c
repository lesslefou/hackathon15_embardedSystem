#include "serialGPS.h"


int convertSerialGpsToTwoCoord(char * string, coord * lat,coord * lon)
{
	if(string == NULL)
		return 0;
	coord longitude,latitude;
	int test = sscanf(string,"GPGGA,%*f,%2d%2d.%2d%2d,%c,%3d%2d.%2d%2d,%c,",&latitude.d,&latitude.m,&latitude.s,&latitude.ms,&latitude.D,&longitude.d,&longitude.m,&longitude.s,&longitude.ms,&longitude.D);
	if(test == 10)
	{
		lat->d = latitude.d;
		lat->m = latitude.m;
		lat->s = latitude.s;
		lat->ms = latitude.ms;
		lat->D = latitude.D;
		lon->d = longitude.d;
		lon->m = longitude.m;
		lon->s = longitude.s;
		lon->ms = longitude.ms;
		lon->D = longitude.D;
		return 1;
	}
	return 0;
}

int closeGPS()
{
	if(streamGPS(-1) == 0)
		return 0;
	sendMessageToGPS(FULL_COLD_START);
	close(streamGPS(-1));
	streamGPS(0);
	return 1;
}

int initGPS(char * location)
{
	struct termios serial;   
	//char * my_serial = "/dev/ttyS0";

    int fd = open(location, O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1) {
        perror(location);
        return 0;
    }

    if (tcgetattr(fd, &serial) < 0) {
        perror("Getting configuration");
        return 0;
    }

    // Set up Serial Configuration
    serial.c_iflag = 0;
    serial.c_oflag = 0;
    serial.c_lflag = 0;
    serial.c_cflag = 0;

    serial.c_cc[VMIN] = 0;
    serial.c_cc[VTIME] = 0;

    serial.c_cflag = B9600 | CS8 | CREAD;

    tcsetattr(fd, TCSANOW, &serial);
	streamGPS(fd);
	
	return 1;
	
}

int streamGPS(int stream)
{
	static int my_stream = 0;
	if(stream >= 0)
		my_stream = stream;
	return my_stream;
}


int sendMessageToGPS(char * message)
{
	if(streamGPS(-1) == 0)
		return 0;
	int wcount = write(streamGPS(-1), message, strlen(message));
    if (wcount < 0) {
        return 0;
    }
	return 1;
}

char * getMessageFromGPS(char * buffer,int max)
{
	if(streamGPS(-1) == 0)
		return NULL;
	char oneCara;
    int rcount;
	int hola = -1;
	do
	{
		rcount = read(streamGPS(-1), &oneCara, sizeof(oneCara));
		if (rcount <= 0 || (hola < 0 && oneCara != '$') || hola >= max) {
			
		}
		else if(oneCara == '$')
		{
			hola = 0;
		}
		else if(oneCara != '\n') {
			buffer[hola++] = oneCara;
		}
		else
		{
			buffer[hola] = '\0';
			return buffer;
		}
	}
	while(rcount >= 0 && hola < max);
	return NULL;
}