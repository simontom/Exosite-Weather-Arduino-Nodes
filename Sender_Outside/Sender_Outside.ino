
// INCLUDES
////////
#include <avr/power.h>
#include <Low-Power\LowPower.h>
#include "NetworkAddresses.h"
#include "Settings.h"
#include "WeatherData.h"
#include "Radio.h"
#include <LEDutilities.h>
#include <DHTEsp32.h>
#include <LiFuelGauge.h>
#include <RH_RF22.h>
#include <RHMesh.h>


// Pins
#define PIN_LED_R 6
#define PIN_LED_G 9
//#define PIN_ANALOG_VOLTAGE A3

#define PIN_SUPPLY_DHT22	8
#define PIN_DATA_DHT_22		7

// Sleeping
#define TIME_ASLEEP	wdt_8s
#define SLEEP_CYCLES_PER_TRANSMISSION	112 /* +- 112 * 8s = 15 minutes */
#define SLEEP_CYCLES_PER_TRANSMISSION_ON_LOW_ENERGY	225 /* +- 225 * 8s = 30 minutes */
#define SLEEP_CYCLES_PER_TRANSMISSION_ON_TOO_LOW_ENERGY	450 /* +- 450 * 8s = 60 minutes */
//#define SLEEP_CYCLES_PER_TRANSMISSION	225 /* +- 225 * 8s = 30 minutes */
//#define SLEEP_CYCLES_PER_TRANSMISSION	300 /* +- 300 * 8s = 40 minutes */
uint16_t sleep_cycles_remaining;

// Sensors
//DHT dht22(PIN_DATA_DHT_22, true);
DHTEsp32 dht;
LiFuelGauge liFuelGauge(MAX17043);

LEDutilities ledG(PIN_LED_G);
LEDutilities ledR(PIN_LED_R);

// Wireless Transceivers
RH_RF22 driver(3, 2);
RHMesh manager(driver, OUTSIDE_NODE_ADDR);

// Watchdog
#define WATCHDOG_ENABLED true	/* MUST STAY FALSE UNTIL CORRECT (Optiboot) BOOTLOADER WITH WDT HANDLING ENABLED IS UPLOADED */
#if WATCHDOG_ENABLED
uint8_t tx_errors_counter = 0;
#endif


void setup(void) {
	// Disables WDT if it is the reason device is starting-up
	#if WATCHDOG_ENABLED
		wdt_disable();
	#endif

	PROTECT_WITH_WDT(
		WATCHDOG_ENABLED,
		initWholeDevice();
	);
}


void loop(void) {
	#if DEBUG_ENABLED && true
		printFreeRam();	////////
	#endif

	if (readSensors()) {
		PROTECT_WITH_WDT(
			WATCHDOG_ENABLED,
			sendData();
		);
	}

	maintainRouting();

	fallAsleep();
}
