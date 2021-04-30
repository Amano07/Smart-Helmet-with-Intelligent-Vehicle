/* gsm.c:	interface program for GSM modem */
#include "gsm.h"

/* static function Declaration */
static	void	GSMerr			(void);
static	int8u	GSMmsgparam		(void);
static	int8u	GSMRxResp		(void);
static	int8u	GSMsndphnum		(const char *PhNum);
static	void	GSMrxack		(void);
static	int8u	GSMReset		(void);
static	int8u	GSMEchoOff		(void);
static	int8u	GSMReadRdy		(void);

/* GSM AT COMMANDS				*/
const char ATCommand[] 		= "AT\r"; 
const char ATCall[]  		= "ATD";
const char SMSCode[]  		= "AT+CMGF=1\r";
const char SMSNumber[] 		= "AT+CMGS=\"";
const char SMSSettings[]	= "AT+CNMI=2,2,2,0,0\r";
const char ATRxCall[]		= "ATA\r";
const char ATDisCall[]		= "ATH\r";
const char ATRST[]			= "AT+CFUN=0\r";
const char ATEchoOFF[]		= "ATE0\r";

extern App Flag;
extern char SerialBuf[];
extern int8u lcdptr;
extern int8u SerlCnt;


void GSMsndmsg(const char *PhNum, const char *GSMMsg) {
	DisUARTInt();
	#ifdef EN_LCD
		lcdclrr(2);
		lcdws("Sending SMS:");
	#endif
	if(GSMsndphnum(PhNum)) {
		puts(MSG_START);
		puts(GSMMsg);
		puts(MSG_END);
		putchar(0x1A);
		GSMrxack();
	} else	GSMerr();
	#ifdef EN_LCD
		lcdws("OK");
	#endif
	//GSMRxResp();
	#ifdef EN_LCD
		dlyms(1000);
		lcdclrr(2);
	#endif
}

int8u GSMinit(void) {
	uint8_t i;
	
		
	i = 0;
	Flag.GSMInitStart = true;
	EnUARTInt();
	
	#ifdef EN_LCD
		lcdclrr(2);
		lcdws("GSM:");
	#endif
	GSMEn();
	
	i = 0;
	while (!(GSMReadRdy())) {
		if (i++ >= 2)
			break;
		GSMReset();
	}
	
	if(Flag.GSMRdy && GSMRxResp() && GSMEchoOff() && GSMmsgparam()) {
		#ifdef EN_LCD
			lcdptr = 0xc4;
			lcdws("            ");
			lcdptr = 0xc4;
			lcdws("OK");
			dlyms(500);
			lcdclrr(2);
		#endif
		Flag.GSMRdy = true;
	} else {
		GSMerr();
		Flag.GSMRdy = false;
	 }
  
	Flag.GSMInitStart = false;
  
	if (Flag.GSMRdy)
		return 1;
	else {
		DisUARTInt();
		return 0; 
	}
}
static int8u GSMReadRdy (void) {
	uint16_t i;
	
	i = 0;
	Flag.Tout = false;
	Flag.GSMRdy = false;
	
	do {
		if (Flag.Msg) {
			Flag.Msg = FALSE;
			lcdptr = 0xc4;
			lcdws("            ");
			lcdptr = 0xc4;
			lcdws(SerialBuf);
			if (!strcmp(SerialBuf, "SMS Ready")) {
			//if (!strcmp(SerialBuf, "Call Ready")) {
				Flag.GSMRdy = true;
				return 1;
			}
		}
		if (i++ >= 250)
			Flag.Tout = true;
		
		dlyms(100);
	} while (!Flag.GSMRdy && !Flag.Tout);
	return 0;
}
static void GSMerr(void){
	#ifdef EN_LCD
		lcdptr = 0xc4;
		lcdws("            ");
		lcdptr = 0xc4;
		lcdws("FAILED!");
		dlyms(1000);
		lcdclrr(2); 
	#endif
}
static int8u GSMRxResp(void) {
	uint8_t i = 0;
	Flag.Tout = false;
	
	SerlCnt = 0;
	Flag.Msg = FALSE;
	
	puts(ATCommand);
	do {
		if (Flag.Msg) {
			Flag.Msg = false;
			if (!strcmp(SerialBuf, "OK"))
				return 1;
		}
		if (i++ >= 10)
			Flag.Tout = true;	
		dlyms(500);
	} while (!Flag.Tout);
	
	return 0;
}

static int8u GSMEchoOff(void) {
	uint8_t i = 0;
	Flag.Tout = false;
	Flag.Msg = false;
	
	SerlCnt = 0;
	Flag.Msg = FALSE;
	
	puts(ATEchoOFF);
	do {
		if (Flag.Msg) {
			Flag.Msg = false;
			if (!strcmp(SerialBuf, "OK"))
			return 1;
		}
		if (i++ >= 10)
			Flag.Tout = true;
		dlyms(500);
	} while (!Flag.Tout);
	
	return 0;
}

static int8u GSMReset (void){
		uint8_t i = 0;
		Flag.Tout = false;
		Flag.Msg = false;
		
		SerlCnt = 0;
		Flag.Msg = FALSE;
		
		puts(ATRST);
		do {
			if (Flag.Msg) {
				Flag.Msg = false;
				if (!strcmp(SerialBuf, "OK")) {
					dlyms(500);
					break;
				}
			}
			if (i++ >= 10)
			Flag.Tout = true;
			dlyms(500);
		} while (!Flag.Tout);
		
		i = 0;
		
		SerlCnt = 0;
		Flag.Msg = FALSE;
		
		puts("AT+CFUN=1\r");
		do {
			if (Flag.Msg) {
				Flag.Msg = false;
				if (!strcmp(SerialBuf, "OK"))
					return 1;
			}
			if (i++ >= 10)
			Flag.Tout = true;
			dlyms(500);
		} while (!Flag.Tout);
		
		return 0;
}
static int8u GSMmsgparam(void) {
	uint8_t i = 0;
	Flag.Tout = false;
	Flag.Msg = false;
	
	SerlCnt = 0;
	Flag.Msg = FALSE;
	
	puts(SMSCode);
	do {
		if (Flag.Msg) {
			Flag.Msg = false;
			if (!strcmp(SerialBuf, "OK"))
				break;
		}
		if (i++ >= 10)
			Flag.Tout = true;
		dlyms(500);
	} while (!Flag.Tout);
	
	if (Flag.Tout)
		return 0;
	
	i = 0;
	Flag.Tout = false;
	
	SerlCnt = 0;
	Flag.Msg = FALSE;
	
	puts(SMSSettings);
	do {
		if (Flag.Msg) {
			Flag.Msg = false;
			if (!strcmp(SerialBuf, "OK"))
				return 1;
			
		}
		if (i++ >= 10)
			Flag.Tout = true;
		dlyms(500);
	} while (!Flag.Tout);
	
	return 0;
}
static int8u GSMsndphnum(const char *PhNum) {
	puts(SMSNumber);
	puts(PhNum);
	puts("\"\r");
	while (getchar() != '>');
	return 1;
}
static void GSMrxack(void) {
	int8u x;
	do {
	 	x = getchar();
	 }while(x != 'K' && x != 'E');
	 if(x == 'E') if(! GSMRxResp() ) GSMerr();
}
int8u gsm_call_gen(const char *PhNum) {
	puts(ATCall);
	puts(PhNum);
	puts(";\r");
	return 1;
}
void GSMRxCall(void) {
	puts(ATRxCall);
}
void GSMDisCall(void) {
	puts(ATDisCall);
}
