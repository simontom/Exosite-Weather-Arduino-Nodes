
#include "Radio.h"
#include "SensorDht22Interier.h"
#include "Settings.h"
#include "Utilities.h"
#include "WeatherData.h"

#include <LEDutilities.h>


// Wireless Transceivers' Addresses
////////
//#define NODE_ADDR	BEDROOM_NODE_ADDR
//#define NODE_ADDR	BATHROOM_NODE_ADDR
//#define NODE_ADDR	LIVROOM_NODE_ADDR

// Hardware configuration
////////
#define LED_PIN				9
#define DHT22_DATA_PIN		A3

WeatherData weatherData;
Radio manager(NODE_ADDR, SLAVE_SELECT_PIN, INTERRUPT_PIN);
SensorDht22Interier sensor(manager, DHT22_DATA_PIN);
LEDutilities led(LED_PIN);

unsigned long lastSendingTime = SENDING_PERIOD;
unsigned long lastLedBlinkTime = LED_BLINKING_PERIOD;
unsigned long now;


void setup(void) {
	#if WATCHDOG_ENABLED
		initializeWdtOnStartup();
	#endif

	#if DEBUG_ENABLED
		Serial.begin(57600);
		printFreeRam(); ////
	#endif

	manager.init(led);

	#if WATCHDOG_ENABLED
		wdt_reset();
	#endif

	led.blinkLed(3, 222);
}

void loop(void) {
	#if WATCHDOG_ENABLED
		wdt_reset();
	#endif

	now = millis();

	TIME_DRIVEN_EVENT(now, lastSendingTime, SENDING_PERIOD,
		if (sensor.readSensors(weatherData)) {
			#if WATCHDOG_ENABLED
				wdt_reset();
			#endif
			manager.sendWeatherData(SINK_NODE_ADDR, weatherData, &led);
		}
	);

	wdt_reset();

	TIME_DRIVEN_EVENT(now, lastLedBlinkTime, LED_BLINKING_PERIOD,
		led.toggleLed(2, 22);
	);

	manager.maintainRouting();

	#if DEBUG_ENABLED && true
		if (Serial.available()) {
			if (Serial.read() == 'i') {
				manager.printRoutingTable();
				Serial.print(F("RSSI: "));
				Serial.println(manager.getLastRssi());
				printFreeRam(); ////
				Serial.println();
			}
			Serial.flush();
		}
	#endif
}
