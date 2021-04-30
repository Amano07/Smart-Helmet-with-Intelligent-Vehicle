/* gps.c: device driver for GPS receiver interface */

static const char PwrOff[] 		= "AT+CGNSPWR=0\r";
static const char PwrOn[] 		= "AT+CGNSPWR=1\r";
static const char Reset[]		= "AT+CNGSRST=1\r";
static const char Info[]  		= "AT+CGNSINF\r";
static const char GPSOut[]		= "AT+CGNSTST=1\r";
static const char GPSStop[]		= "AT+CGNSTST=0\r";
static const char GPSGGA[]		= "AT+CGNSSEQ=\"GGA\"\r";

#include"gps.h"

void GPSInit(void) {
	 GSM_GPS_DDR |= _BV(GSM_GPS_PIN);
	 GSM_GPS_PORT &= ~_BV(GSM_GPS_PIN); 
	/* puts(PwrOn);
	dlyms(100); */
} 
void GPSgetloc(int8u *Lat, int8u *Lon, int8u *Speed) {
	int8u i, x;
//	puts(GPSOut);
		
ReadAgain:
	while (getchar() != '$');
	
	if(getchar() != 'G')
		goto ReadAgain;
	if(getchar() != 'P')
 		goto ReadAgain;
	if(getchar() != 'R')		/* Ealier GPGGA */
		goto ReadAgain;
	if(getchar() != 'M')
		goto ReadAgain;
	if(getchar() != 'C')
		goto ReadAgain;
		
	while(getchar() != ',');
	while(getchar() != ',');
	while(getchar() != ',');
	
	for(i = 0; i < 9; i++)
		*Lat++ = getchar();
		
	while(((x = getchar()) != 'N') && x != 'S');
	
	//while(getchar() != ',');
	while(getchar() != ',');
	for(i = 0; i < 10; i++)
		*Lon++ = getchar();
		
	while(((x = getchar()) != 'E') && x != 'W');
	
	//while(getchar() != ',');
	while(getchar() != ',');
	
	for(i = 0; i < 5; i++)
		*Speed++ = getchar();
	
//	puts(GPSStop);	
} 
/*
void GPSgetloc(int8u *Lat, int8u *Lon) {
	int8u i;
	puts(Info);
	
	ReadAgain:
	while(getchar() != '+');
	if(getchar() != 'C')
	goto ReadAgain;
	if(getchar() != 'G')
	goto ReadAgain;
	if(getchar() != 'N')
	goto ReadAgain;
	if(getchar() != 'S')
	goto ReadAgain;
	if(getchar() != 'I')
	goto ReadAgain;
	if(getchar() != 'N')
	goto ReadAgain;
	if(getchar() != 'F')
	goto ReadAgain;
	while(getchar() != ',');
	while(getchar() != ',');
	while(getchar() != ',');
	for(i = 0; i < 9; i++)
		*Lat++ = getchar();
	while(getchar() != ',');
	for(i = 0; i < 10; i++)
		*Lon++ = getchar();
} */
