#ifndef SERIALGPS
#define SERIALGPS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
//https://www.rhydolabz.com/documents/25/NMEA-CommandManual_%28FTX-HW-13002%29.pdf
#define PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ                                    \
  "$PMTK220,10000*2F\r\n" ///< Once every 10 seconds, 100 millihertz.
#define PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ                                    \
  "$PMTK220,5000*1B\r\n" ///< Once every 5 seconds, 200 millihertz.
#define PMTK_SET_NMEA_UPDATE_1HZ "$PMTK220,1000*1F\r\n" ///<  1 Hz
#define PMTK_SET_NMEA_UPDATE_2HZ "$PMTK220,500*2B\r\n"  ///<  2 Hz
#define PMTK_SET_NMEA_UPDATE_5HZ "$PMTK220,200*2C\r\n"  ///<  5 Hz
#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F\r\n" ///< 10 Hz
// Position fix update rate commands.
#define PMTK_API_SET_FIX_CTL_100_MILLIHERTZ                                    \
  "$PMTK300,10000,0,0,0,0*2C\r\n" ///< Once every 10 seconds, 100 millihertz.
#define PMTK_API_SET_FIX_CTL_200_MILLIHERTZ                                    \
  "$PMTK300,5000,0,0,0,0*18\r\n" ///< Once every 5 seconds, 200 millihertz.
#define PMTK_API_SET_FIX_CTL_1HZ "$PMTK300,1000,0,0,0,0*1C\r\n" ///< 1 Hz
#define PMTK_API_SET_FIX_CTL_5HZ "$PMTK300,200,0,0,0,0*2F\r\n"  ///< 5 Hz
// Can't fix position faster than 5 times a second!

#define PMTK_SET_NMEA_OUTPUT_GGAONLY                                           \
  "$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29\r\n" ///< turn on just the
                                                      ///< GPGGA
#define PMTK_SET_NMEA_OUTPUT_ALLDATA                                           \
  "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n" ///< turn on ALL THE DATA
#define PMTK_SET_NMEA_OUTPUT_OFF                                               \
  "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28\r\n" ///< turn off output

// to generate your own sentences, check out the MTK command datasheet and use a
// checksum calculator such as the awesome
// http://www.hhhh.org/wiml/proj/nmeaxor.html

#define PMTK_LOCUS_STARTLOG "$PMTK185,0*22\r\n" ///< Start logging data
#define PMTK_LOCUS_STOPLOG "$PMTK185,1*23\r\n"  ///< Stop logging data
#define PMTK_LOCUS_STARTSTOPACK                                                \
  "$PMTK001,185,3*3C\r\n" ///< Acknowledge the start or stop command
#define PMTK_LOCUS_QUERY_STATUS "$PMTK183*38\r\n"  ///< Query the logging status
#define PMTK_LOCUS_ERASE_FLASH "$PMTK184,1*22\r\n" ///< Erase the log flash data

#define PMTK_ENABLE_SBAS                                                       \
  "$PMTK313,1*2E\r\n" ///< Enable search for SBAS satellite (only works with 1Hz
                  ///< output rate)
#define PMTK_ENABLE_WAAS "$PMTK301,2*2E\r\n" ///< Use WAAS for DGPS correction data

#define PMTK_STANDBY                                                           \
  "$PMTK161,0*28" ///< standby command & boot successful message
#define PMTK_STANDBY_SUCCESS "$PMTK001,161,3*36\r\n" ///< Not needed currently
#define PMTK_AWAKE "$PMTK010,002*2D\r\n"             ///< Wake up

#define PMTK_Q_RELEASE "$PMTK605*31\r\n" ///< ask for the release and version

#define PGCMD_ANTENNA                                                          \
  "$PGCMD,33,1*6C" ///< request for updates on antenna status
#define PGCMD_NOANTENNA "$PGCMD,33,0*6D\r\n" ///< don't show antenna status messages

#define FULL_COLD_START "$PMTK104*37\r\n" //reset

typedef struct coord
{
	int d;
	int m;
	int s;
	int ms;
	char D;
} coord;

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
int convertSerialGpsToTwoCoord(char * string, coord * lat,coord * lon);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
int closeGPS();

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
int initGPS(char * location);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
int streamGPS(int stream);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
char * getMessageFromGPS(char * buffer,int max);

/*	@description:
 *	@param:
 *	@param:
 *	@return:
*/ 
int sendMessageToGPS(char * string);

#endif