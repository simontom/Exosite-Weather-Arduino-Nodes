
#ifndef _UTILITIES_h
#define _UTILITIES_h


#include "WeatherData.h"
#include "Settings.h"
#include <Arduino.h>
#include <avr/wdt.h>


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

#if WATCHDOG_ENABLED
#define PROTECT_WITH_WDT(_body_)	\
	do { \
		wdt_reset(); \
		wdt_enable(WATCHDOG_RESET_TIME); \
		_body_ \
		wdt_disable(); \
	} while(false)
#else
	#define PROTECT_WITH_WDT(_body_)	\
	do { \
		_body_ \
	} while(false)
#endif


// Helper Functions
/////////////////////////////////////////////////////////

#if WATCHDOG_ENABLED
extern void initializeWdtOnStartup(void);
#endif

extern uint16_t getVoltage_mV(uint8_t voltagePin);

extern void rebootDevice(void);

#if DEBUG_ENABLED
extern void printWeatherData(WeatherData &data);
extern int getFreeRam(void);
extern void printFreeRam(void);
#endif

#endif
