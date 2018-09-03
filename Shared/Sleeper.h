#ifndef _SLEEPER_h
#define _SLEEPER_h


#include "Radio.h"
#include "Utilities.h"
#include "WeatherData.h"
#include "Settings.h"

#include <LowPower.h>


// Settings
/////////////////////////////////////////////////////////
#define SLEEP_CYCLES_PER_TRANSMISSION					112 /* +- 112 * 8s = 15 minutes */
//#define SLEEP_CYCLES_PER_TRANSMISSION					225 /* +- 225 * 8s = 30 minutes */
//#define SLEEP_CYCLES_PER_TRANSMISSION					300 /* +- 300 * 8s = 40 minutes */
#define SLEEP_CYCLES_PER_TRANSMISSION_ON_LOW_ENERGY		225 /* +- 225 * 8s = 30 minutes */
#define SLEEP_CYCLES_PER_TRANSMISSION_ON_TOO_LOW_ENERGY	450 /* +- 450 * 8s = 60 minutes */

#define TIME_ASLEEP WDTO_8S


class Sleeper {
	
	public:
		Sleeper(const Radio &manager);
		void sleep(const uint16_t milivolts);

	private:
		const Radio &manager;
		uint16_t sleep_cycles_remaining;

		inline uint16_t getSleepCycles(const uint16_t milivolts);

};

#endif
