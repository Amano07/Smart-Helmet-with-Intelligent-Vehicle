#include "sms.h"

extern int8u OTP[5];
extern App Flag;
char RxPhNum[15], sbuf[20];
char SerialBuf[200];
extern uint8_t lcdptr;
int8u SerlCnt = 0;


void SendOTP(const char *PNum,const char *GSMMsg) {
	int8u gsmmsg[150];
	gsmmsg[0] = '\0';
	genOTP(OTP);
	strcat(gsmmsg,GSMMsg);
	strcat(gsmmsg,OTP);
	GSMEn();
	GSMsndmsg(PNum, gsmmsg);
}

void sendloc(const char *PhNum, const char *GSMMsg) {
	int8u i;
	int8u lat[11], lon[11], Speed[10];
	int8u gsmmsg[150];

	gsmmsg[0] = '\0';

	for (i = 0; i < 10; i++)
		lat[i] = lon[i] = '0';

	lat[10] = lon[10] = '\0';

	GPSEn();
	GPSgetloc(lat,lon, Speed);

	strcat(gsmmsg,GSMMsg);
	strcat(gsmmsg," LAT:");
	strcat(gsmmsg,lat);
	strcat(gsmmsg," & LON: ");
	strcat(gsmmsg,lon);

	GSMEn();
	GSMsndmsg(PhNum, gsmmsg);
}

void SendLinkLoc(const char *PhNum, const char *GSMMsg) {
	int8u i;
	int8u lat[11], lon[11], speed[10];
	int8u gsmmsg[150];
	int8u latl[3];
	int8u latu[10];	
	int8u lonl[4];
	int8u lonu[9];
	
	float latdec, londec, SpeedInKmh;

	gsmmsg[0] = '\0';

	for (i = 0; i < 10; i++)
		lat[i] = lon[i] = '0';

	lat[10] = lon[10] = speed[5] = '\0';
	lat[0] = lon[0] = speed[0] = '\0';
	

	strcat(gsmmsg,GSMMsg);
	strcat(gsmmsg, "Located @ http://maps.google.com/maps?q=");
	
	DisUARTInt();
	
	#ifdef DEMO
		strcat(gsmmsg,"12.2766");
		strcat(gsmmsg,",");
		strcat(gsmmsg,"76.62022");
	#else	
		GPSEn();
		GPSgetloc(lat,lon, speed);
		
		#ifdef DISPLAY_LOC
			lcdclr();
			lcdws(lat);
			lcdr2();
			lcdws(lon);
			dlyms(2000);
			lcdclr();
		#endif 
		
		#ifdef DISPLAY_SPEED
			lcdclr();
			lcdws(speed);
		#endif
			
		for (i = 0; i < 2; i++)
			latl[i] = lat[i];
		latl[i] = '\0';
		for (i = 2; i < 10; i++)
			latu[i-2] = lat[i];
		latu[i-2] = '\0';
				
		latdec = (atof(latl)) +  ((atof(latu)) / 60.0);
		ftoa(latdec, lat, 4);
			
		for (i = 0; i < 3; i++)
			lonl[i] = lon[i];
		lonl[i] = '\0';
		for (i = 3; i < 10; i++)
			lonu[i-3] = lon[i];
		
		lonu[i-3] = '\0';
			
		londec = atof(lonl) + ( atof(lonu) / 60.0 );
		ftoa(londec, lon,4); 
		
		SpeedInKmh = atof(speed) * 1.852;
		ftoa(SpeedInKmh, speed, 1);			
		strcat(gsmmsg,lat);
		strcat(gsmmsg,",");
		strcat(gsmmsg,lon); 
		
		strcat(gsmmsg, " Speed:");
		strcat(gsmmsg, speed);
		strcat(gsmmsg, " in Kmh");
		
	#endif
	
	GSMEn();
	GSMsndmsg(PhNum, gsmmsg);
}
ISR (USART_RXC_vect) {
	static int8u i, j;
	static int8u state = MSG_WAIT_MSG;
	
	if (!Flag.GSMInitStart) {
		switch (state) {
			case MSG_WAIT_MSG:
				if (UDR == 0x22) 	
					state = MSG_PH_NUM;
				break;
			case MSG_PH_NUM:
				if (i < 13)
					RxPhNum[i++] = UDR;
				else
					state = MSG_COLL_MSG;
				break;
			case MSG_COLL_MSG:
				if (UDR == LINE_FEED)
					state = MSG_RCV_MSG;
				break;
			case MSG_RCV_MSG:
				if ((sbuf[j++] = UDR) == LINE_FEED) {
					sbuf[j-2] = '\0';
					#ifdef ELIMINATE_+91
						for (i = 0 ; i < 10; i++)	/* eliminate +91 */
							RxPhNum[i] = RxPhNum[i+3];
						RxPhNum[i] = '\0';
					#else
						RxPhNum[13] = '\0';
					#endif
					state = MSG_WAIT_MSG;
					i = j = 0;
					Flag.Msg = TRUE;
					DisUARTInt();
				}
		}
	} else {
		if ((SerialBuf[SerlCnt++] = UDR) == LINE_FEED){
			Flag.Msg = true;
			SerialBuf[SerlCnt-2] = '\0';
			SerlCnt = 0;
		} 
	} 
			
}