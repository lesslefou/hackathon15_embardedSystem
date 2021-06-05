#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

typedef struct l 
{
	int d;
	int m;
	int s;
	int ms;
	char D;
} l;


void lire_le_truc(char * string, int taille)
{
	l longitude,latitude;
	puts(string);
	int yo = sscanf(string,"GPGGA,%*f,%2d%2d.%2d%2d,%c,%3d%2d.%2d%2d,%c,",&latitude.d,&latitude.m,&latitude.s,&latitude.ms,&latitude.D,&longitude.d,&longitude.m,&longitude.s,&longitude.ms,&longitude.D);
	printf("latitude :%d|%d|%d|%c\nlongitude:%d%c et %i\n",latitude.d,latitude.m,latitude.s,latitude.D,longitude.d,longitude.D,yo);
}

int main() {

    struct termios serial;
    
	

	char * my_serial = "/dev/ttyS0";

    printf("Opening %s\n", my_serial);

    int fd = open(my_serial, O_RDWR | O_NOCTTY | O_NDELAY);

    if (fd == -1) {
        perror(my_serial);
        return -1;
    }

    if (tcgetattr(fd, &serial) < 0) {
        perror("Getting configuration");
        return -1;
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

	char oneCara;
    int rcount;
	char buffer[255];
	int hola = -1;
	do
	{
		rcount = read(fd, &oneCara, sizeof(oneCara));
		if (rcount <= 0 || (hola < 0 && oneCara != '$') || hola > 254) {
			
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
			hola = -1;
			lire_le_truc(buffer,hola);
			
			fflush(stdout);
		}
	}
	while(rcount >= 0);



    close(fd);
}