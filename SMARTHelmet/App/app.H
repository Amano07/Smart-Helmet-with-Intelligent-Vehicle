#ifndef _APP_H
#define _APP_H

#include "types.h"

/* Main settings */
//#define TESTING
//#define DEMO


/********** GPS & GSM MUX CONFIGURATION ***********/
#define GSM_GPS_DDR				DDRD
#define GSM_GPS_PORT			PORTD
#define GSM_GPS_PIN				PD6

/******** GPS & GSM MUX CONFIGURATION ENDS *******/

/* LCD CONFIGURATION */
#define EN_LCD
#define LCD_DDR           	   		DDRC 
#define LCD_DATA_PORT          		PORTC
#define LCD_EN		        		PC3 
#define LCD_RS						PC2
#define DISPLAY_INIT				1
#define LCD_NSCRL					3
#define FLOAT_T0_STRING				1
#define DECIMAL_PRECISION			10

/******** LCD CONFIGURATION ENDS ****************/

/* BUZZER: CONFIGURATION */
#define	BUZ_DDR						DDRC
#define	BUZ_PORT					PORTC
#define	BUZ_PIN						PC1
#define OFF_TIME					100
/******** BUZZER CONFIGURATION ENDS ****************/

/* LED: CONFIGURATION */

#define	LED_DDR			DDRC
#define	LED_PORT		PORTC
#define	LED_PIN			PC0
#define LED1_PIN		LED_PIN
#define OFF_TIME		100
/******** LED CONFIGURATION ENDS ****************/

/* GSM: CONFIGURATION */
#ifdef TESTING
	#define RPhNum	 "8851762880"
#else
	#define RPhNum	 "9716622541"
#endif

#define MSG_START		""
#define MSG_END			". Automated SMS By:SH"

#define GSMEn()				do {										\
									GSM_GPS_PORT &= ~_BV(GSM_GPS_PIN); 	\
									dlyms(500);							\
								} while(0)

#define GPSEn()				do {									\
								GSM_GPS_PORT |= _BV(GSM_GPS_PIN);	\
								dlyms(500);							\
							} while(0)

/******** ADC CONFIGURATION ENDS ****************/
#define REF_VOL_256

/******** EEPROM CONFIGURATION ****************/
#define EEPROM_DISPLAY		0

/******** MOTOR CONFIGURATION *****************/
#define MOT_DDR			DDRD
#define MOT_PORT		PORTD
#define M1_AN			PD7
#define M1_CA			PD4
#define M2_AN			PD5
#define M2_CA			PD6


/****** RF CONFIGURATION ********/
#define RF_RECEIVER_ENABLE
#define RFDDR			DDRB
#define RFPORT			PORTB
#define RFPPIN			PINB
#define RFVDPIN			PB0
#define RFDATA3			PB1
#define RFEN_PIN		PB2
/****** RF CONFIGURATION ENDS  *****/

/* DEFINE Flag Structure Definition */
typedef struct  {
	volatile int8u sw			:1;
	volatile int8u Check		:1;
	volatile int8u HelmetOn		:1;
	volatile int8u HelmetOff	:1;
	volatile int8u state		:1;
	volatile int8u HelSig		:1;
	volatile int8u Moving		:1;
	volatile int8u Msg			:1;
	volatile int8u MotorOn		:1;
	volatile int8u Vibr			:1;
	volatile int8u Temp			:1;
	volatile int8u GSMRdy		:1;
	volatile int8u Tout			:1;
	volatile int8u GSMInitStart	:1;
}App;

#endif
