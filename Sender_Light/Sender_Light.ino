
// INCLUDES
////////
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#include "NetworkAddresses.h"
#include "Settings.h"
#include "WeatherData.h"
#include <SPI.h>
#include "I2C_Rev5\I2C.h"
#include <LEDutilities.h>
#include <BH1750FVI.h>
#include <UVindex.h>
#include <RH_RF22.h>
#if USE_MESH_LIBRARY
#include <RHMesh.h>
#else
#include <RHRouter.h>
#endif


// Pins
#define ledPinR 5
#define ledPinG 9
#define voltagePin A3
#define uvIndexSensorSupplyPin A0
#define LIGHTSENSOR_ADDR_PIN A2
#define UVINDEX_PIN A1

// Sleeping
#define TIME_ASLEEP	wdt_8s
//#define sleep_cycles_per_transmission	300 /* +- 300 * 8s = 40 minutes // 225 for 30 mins // 263 * 8s = 35 */
#define sleep_cycles_per_transmission	450 /* +- 450 * 8s = 60 minutes */
uint16_t sleep_cycles_remaining;

// Sensors
BH1750FVI LightSensor;
UVindex uvIndex(UVINDEX_PIN);

LEDutilities ledG(ledPinG);
LEDutilities ledR(ledPinR);

// Wireless Transceivers
RH_RF22 driver(3, 2);
#if USE_MESH_LIBRARY
RHMesh manager(driver, LIGHT_NODE_ADDR);
#else
RHRouter manager(driver, LIGHT_NODE_ADDR);
#endif

// Watchdog
#define WATCHDOG_ENABLED true	/* MUST STAY FALSE UNTIL BOOTLOADER NOT UPLOADED */
#if WATCHDOG_ENABLED
uint8_t tx_errors_counter = 0;
#endif

//// Debug Additions ////
#define DEBUG_ENABLED			false
#define STOP_HERE_BLINK()			while(true) { ledG.blinkLed(1, 444, 1555); }
#define DEBUG_WITH_LED_GREEN(body)	do { \
									ledG.onLed(); \
									body; \
									ledG.offLed(); \
								} while(false)

#define DEBUG_WITH_LED_RED(body)	do { \
									ledR.onLed(); \
									body; \
									ledR.offLed(); \
								} while(false)


void setup(void) {
	#if WATCHDOG_ENABLED
		wdt_disable();
	#endif

	#if DEBUG_ENABLED && true
		Serial.begin(57600);
		printFreeRam(); ////
	#endif

	#if !(DEBUG_ENABLED && true)
			power_usart0_disable();
	#endif

	LightSensor.begin(MODE_OneTime_High_Res2, Address_H, LIGHTSENSOR_ADDR_PIN);

	ledG.blinkLed(2, 222);
	ledR.blinkLed(2, 222);
}


void loop(void) {
	#if DEBUG_ENABLED && true
		printFreeRam();	////////
	#endif

	readSensors();
	sendData();
	fallAsleep();
}
