#include "main.h"

extern int8u lcdptr;

App Flag;

int main(void) {
	init();
	while (TRUE) {
		if (Flag.Check) {
			ChkHelm();
			if (Flag.Moving)
				ChkAlco();
			Flag.Check = FALSE;
		}
		if (Flag.Vibr) 
			Accident();
		if (Flag.sw ) 
			IgnSw();
		sleep();	
	}
	return 0;
}
static void init(void) {
	motorinit();
	buzinit();
	ledinit();
	beep(2,100);
	lcdinit();
	uartinit();
	sei();
	EXTINTinit();
	GPSInit();
	GSMEn();
	GSMinit();
	tmr1init();
	RFRxInit();
	disptitl();
	sleep();
}
static void Accident(void) {
	Flag.Vibr = FALSE;
	lcdclrr(2);
	lcdws("Accident Occur'd");
	motoff();
	beep(1,250);
	dlyms(1000);
	SendLinkLoc(RPhNum,"Your Vehicle Got Accident at ");
	lcdclr();
	lcdws("Accident Occur'd");
	Reset();
}
static void IgnSw(void) {
	if (Flag.HelmetOn){
		beep(1,100);
		if (!Flag.state) {
			Flag.state = TRUE;
			moton();
			lcdclrr(2);
			lcdws(" Vehicle Moving");
			Flag.Moving = TRUE;
		} else {
			Flag.state = FALSE;
			motoff();
			lcdclrr(2);
			lcdws(" Vehicle Stopped");
			Flag.Moving = FALSE;
		}
	}
	Flag.sw = FALSE;
}
static void ChkHelm(void) {
	if (RFPPIN & _BV(RFVDPIN)) {
		if (!Flag.HelmetOn) {
			Flag.HelmetOn = TRUE;
			lcdclrr(2);
			lcdws(" Helmet Weared");
			beep(1,150);
			GICR |= _BV(INT1);			/* ENABLE EXTERNAL INTERRUPT */
		}
	} else {
		if (Flag.HelmetOn) {
			Flag.HelmetOn = FALSE;
			Flag.state = FALSE;
			Flag.Moving = FALSE;
			lcdclrr(2);
			lcdws(" Helmet Removed");
			GICR &=  ~_BV(INT1);		/* Disable Switch */
			motoff();
			beep(1,150);
			dlyms(1000);
			lcdclrr(1);
			lcdws(" Vehicle Stopped");
			dlyms(1000);
		}
	}
}
static void ChkAlco(void) {
	if ((RFPPIN & _BV(RFDATA3)) == 0) 	{
		lcdclr();
		lcdws("Alcohol Consumed");
		motoff();
		GICR &=  ~_BV(INT1);		/* Disable Switch */
		beep(1,250);
		dlyms(1000);
		Reset();
	} 
}
static void Reset(void) {
	lcdclrr(2);
	lcdws(" Reset to START");
	beep(2,500);
	cli();
	ledon();
	sleep();
	for(;;);
}
static void EXTINTinit(void) {
	INTDDR 	&= ~(_BV(INT0_PIN) | _BV(INT1_PIN));
	INTPORT |= (_BV(INT0_PIN) | _BV(INT1_PIN));

	MCUCR	|= _BV(ISC01);					/* FALLING EDGE INTERRUPT FOR SWITCH */
	MCUCR	|= _BV(ISC10) | _BV(ISC11);		/* Rising Edge Interrupt for Vibration Sensor */
	
	GICR	|= _BV(INT1);						/* ENABLE EXTERNAL INTERRUPT Accident 	*/
	GICR	|= _BV(INT0);						/* Disable Switch */
	
	GIFR |= _BV(INTF1);						/* Clear Early Interrupt */

}
static void disptitl(void) {
	lcdclr();
	lcdws("  SMART HELMET");
}
static void Flagsinit(void) {
	Flag.HelmetOn = FALSE;
	Flag.Check = FALSE;
	Flag.Moving = FALSE;
	Flag.state = FALSE;
	Flag.sw = FALSE;
	Flag.Vibr = FALSE;
}		
ISR(INT0_vect) { 
	Flag.sw = TRUE;
	GICR |= _BV(INT1);
}

ISR(INT1_vect) {
	Flag.Vibr = TRUE;
	GICR |= _BV(INT0);
}
static void tmr1init(void) {
	TCNT1H   = 225;					/* overflows at every 250msec */
	TCNT1L   = 123;
	TIMSK   |= _BV(TOIE1);			/* ENABLE OVERFLOW INTERRUPT */
	TCCR1A   = 0x00;
	TCCR1B  |= _BV(CS12);			 /* PRESCALAR BY 256 */
}
/* overflows at every 250msec */
ISR(TIMER1_OVF_vect) {
	static int8u i, j, k;
	TCNT1H   = 225;					/* overflows at every 250msec */
	TCNT1L   = 123;
	if (!Flag.Check && ++j >= 2) {
		Flag.Check = TRUE;
		j = 0;
	}
	if (++i >= 20)	i = 0;
	switch(i) {
		case 0: case 2: ledon(); break;
		case 1: case 3: ledoff(); break;
	}
}