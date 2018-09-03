
#include "NetworkAddresses.h"
#include "Radio.h"
#include "Settings.h"
#include "Utilities.h"
#include "WeatherData.h"
#include "Sleeper.h"
#include "SensorDht22Exterier.h"

#include <avr/power.h>
#include <LEDutilities.h>


// Pins
#define LED_RED_PIN			6
#define LED_GREEN_PIN		9
#define SUPPLY_DHT22_PIN	8
#define DATA_DHT_22_PIN		7

WeatherData weatherData;
Radio manager(OUTSIDE_NODE_ADDR, SLAVE_SELECT_PIN, INTERRUPT_PIN);
Sleeper sleeper(manager);
SensorDht22Exterier sensors(manager, DATA_DHT_22_PIN, SUPPLY_DHT22_PIN);

LEDutilities redLed(LED_RED_PIN);
LEDutilities greenLed(LED_GREEN_PIN);


void setup(void) {
	#if WATCHDOG_ENABLED
	initializeWdtOnStartup();
	#endif

	#if DEBUG_ENABLED
	Serial.begin(57600);
	printFreeRam();
	#endif

	PROTECT_WITH_WDT(
		#if !DEBUG_ENABLED
		power_usart0_disable();
		#endif
		//power_twi_disable();

		greenLed.blinkLed(2, 222);
		manager.init(redLed);
		redLed.blinkLed(2, 222);

		greenLed.offLed();
		redLed.offLed();
	);
}


void loop(void) {
	#if DEBUG_ENABLED
	printFreeRam();
	#endif

	if (sensors.readSensors(weatherData)) {
		#if DEBUG_ENABLED
		PROTECT_WITH_WDT(
			manager.sendData(SINK_NODE_ADDR, weatherData, &redLed);
		);
		#else
		PROTECT_WITH_WDT(
			manager.sendData(SINK_NODE_ADDR, weatherData);
		);
		#endif
	}

	manager.maintainRouting();

	sleeper.sleep(weatherData.getMilivolts());
}
