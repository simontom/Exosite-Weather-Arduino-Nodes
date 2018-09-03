#ifndef _SLEEPER_h
#define _SLEEPER_h


#include "Radio.h"
#include "Utilities.h"
#include "WeatherData.h"
#include "Settings.h"

#include <Arduino.h>
#include <avr/wdt.h>
#include <Low-Power\LowPower.h>


// Settings
/////////////////////////////////////////////////////////
#define SLEEP_CYCLES_PER_TRANSMISSION					112 /* +- 112 * 8s = 15 minutes */
//#define SLEEP_CYCLES_PER_TRANSMISSION					225 /* +- 225 * 8s = 30 minutes */
//#define SLEEP_CYCLES_PER_TRANSMISSION					300 /* +- 300 * 8s = 40 minutes */
#define SLEEP_CYCLES_PER_TRANSMISSION_ON_LOW_ENERGY		225 /* +- 225 * 8s = 30 minutes */
#define SLEEP_CYCLES_PER_TRANSMISSION_ON_TOO_LOW_ENERGY	450 /* +- 450 * 8s = 60 minutes */

#define TIME_ASLEEP wdt_8s

// Helper Definitions
/////////////////////////////////////////////////////////
typedef enum {
	wdt_16ms = 0,
	wdt_32ms,
	wdt_64ms,
	wdt_128ms,
	wdt_250ms,
	wdt_500ms,
	wdt_1s,
	wdt_2s,
	wdt_4s,
	wdt_8s
} wdt_prescalar_e;


class Sleeper {
	
	public:
		Sleeper(const Radio &manager);
		inline void sleep(const WeatherData &data);

	private:
		const Radio &manager;
		uint16_t sleep_cycles_remaining;

		inline uint16_t getSleepCycles(const WeatherData &data);

};

#endif
