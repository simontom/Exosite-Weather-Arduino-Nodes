#ifndef _UTILITIES_h
#define _UTILITIES_h

#include <arduino.h>
#include <avr/wdt.h>

//typedef enum {
//	wdt_16ms = 0,
//	wdt_32ms,
//	wdt_64ms,
//	wdt_128ms,
//	wdt_250ms,
//	wdt_500ms,
//	wdt_1s, wdt_2s,
//	wdt_4s, wdt_8s
//} wdt_prescalar_e;

#define WATCHDOG_RESET_TIME	WDTO_8S

#define STOP_HERE()				while(true) { ; }
#define STOP_HERE_LED(_LED_)	while(true) { _LED_.blinkLed(1, 444, 1555); }
#define DEBUG_WITH_LED(_led_, _body_)	\
	do { \
		_led_.onLed(); \
		_body_; \
		_led_.offLed(); \
	} while(false)

#define TIME_DRIVEN_EVENT(now_, last_, period_, body_)	\
	do { \
		if ((now_ - last_) >= (period_)) { \
			last_ = now_; \
			body_ \
		} \
	} while (false)

#define PROTECT_WITH_WDT(_wdt_enabled_, _body_)	\
	do { \
		if (_wdt_enabled_) { \
			wdt_reset(); \
			wdt_enable(WATCHDOG_RESET_TIME); \
		} \
		_body_ \
		if (_wdt_enabled_) { \
			wdt_disable(); \
		} \
	} while(false)


// Helper Functions
/////////////////////////////////////////////////////////

extern void rebootDevice(void);
extern int getFreeRam(void);
extern void printFreeRam(void);

#endif
