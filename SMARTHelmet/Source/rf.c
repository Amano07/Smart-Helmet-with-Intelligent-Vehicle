#include "rf.h"

#ifdef RF_RECEIVER_ENABLE
	void RFRxInit(void) {
		RFDDR &= 0x0F;
		RFPORT |= 0xF0;
		#ifdef RFVD_ENABLE
			RFDDR &= ~_BV(RFVD_PIN);
			RFPORT |= _BV(RFVD_PIN);
		#endif
	}
	int8u RFRx(void) {
		if (RFPPIN & _BV(RFVDPIN))
			return ((RFPPIN & 0xF0) >> 4);
		else
			return 0;
	}
#endif

#ifdef RF_TRANSMITTER_ENABLE
	void RFTxInit(void) {
		RFDDR |= _BV(RFENPIN);
		RFPORT |= _BV(RFENPIN);
		RFDDR |= 0xF0;
		RFPORT &= 0x0F;
		
	}
	void RFTx(int8u x){
		RFPORT = (RFPORT & 0x0F) | (x << 4);
		EnRF();
		RFNull();
	}
	void trans(const char *str) {
		int8u i;
		rfinit();
		RFPORT = (RFPORT & 0X0F) | ('<' & 0xF0);
		EnRF();
		RFPORT = (RFPORT & 0X0F) | ('<' << 4);
		EnRF();
		for (i = 0; str[i] != '\0'; i++) {
			RFPORT = RFPORT & 0X0F | (str[i] & 0xF0);
			EnRF();
			RFPORT = RFPORT & 0X0F | (str[i] << 4);
			EnRF();
		}
		RFPORT = RFPORT & 0X0F | ('>' & 0xF0);
		EnRF();
		RFPORT = RFPORT & 0X0F | ('>' << 4);
		EnRF();
		
		RFNull();
	}
	
#endif

#ifdef  RF_ENABLE_ONLY
	void RFEnInit(void) {
		RFDDR |= _BV(RFEN_PIN);
		RFPORT |= _BV(RFEN_PIN);
	}
#endif
