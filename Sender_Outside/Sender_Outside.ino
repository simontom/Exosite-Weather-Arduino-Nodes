
// INCLUDES
////////
#include <avr/power.h>
#include <Low-Power\LowPower.h>
#include "NetworkAddresses.h"
#include "Radio.h"
#include "Settings.h"
#include "Utilities.h"
#include "WeatherData.h"
#include "Sleeper.h"
#include <LEDutilities.h>
#include <DHTEsp32.h>
#include <LiFuelGauge.h>
#include <RH_RF22.h>
#include <RHMesh.h>


// Pins
#define LED_R_PIN			6
#define LED_G_PIN			9
#define SUPPLY_DHT22_PIN	8
#define DATA_DHT_22_PIN		7

Radio manager(OUTSIDE_NODE_ADDR, SLAVE_SELECT_PIN, INTERRUPT_PIN);
Sleeper sleeper(manager);

// Sensors
WeatherData weatherData;
DHTEsp32 dht;
LiFuelGauge liFuelGauge(MAX17043);

LEDutilities ledR(LED_R_PIN);
LEDutilities ledG(LED_G_PIN);

void setup(void) {
	#if WATCHDOG_ENABLED
		initializeWdtOnStartup();
	#endif

	#if DEBUG_ENABLED
		Serial.begin(57600);
		printFreeRam(); ////
	#endif

	PROTECT_WITH_WDT(
		#if !DEBUG_ENABLED
			power_usart0_disable();
		#endif
		//power_twi_disable();

		ledG.blinkLed(2, 222);
		manager.init(ledR);
		//liFuelGauge.reset();
		ledR.blinkLed(2, 222);

		ledG.offLed();
		ledR.offLed();
	);
}


void loop(void) {
	#if DEBUG_ENABLED
		printFreeRam();
	#endif

	if (readSensors()) {
		PROTECT_WITH_WDT(
			manager.sendData(SINK_NODE_ADDR, weatherData);
		);
	}
	manager.maintainRouting();
	sleeper.sleep(weatherData);
}
