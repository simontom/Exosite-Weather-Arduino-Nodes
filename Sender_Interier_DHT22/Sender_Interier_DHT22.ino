
// INCLUDES
////////////
#include "NetworkAddresses.h"
#include "Radio.h"
#include "Settings.h"
#include "Utilities.h"
#include "WeatherData.h"
#include <SPI.h>
#include <LEDutilities.h>
#include <DHT_LITE.h>


// Wireless Transceivers' Addresses
////////
//#define NODE_ADDR	BEDROOM_NODE_ADDR
//#define NODE_ADDR	BATHROOM_NODE_ADDR
#define NODE_ADDR	LIVROOM_NODE_ADDR

// Hardware configuration
////////
// Pins
#define LED_PIN				9
#define DHT22_DATA_PIN		A3
// Sensors
WeatherData weatherData;
DHT_LITE dht22(DHT22_DATA_PIN, false, DHT22);
LEDutilities led(LED_PIN);
Radio manager(NODE_ADDR, SLAVE_SELECT_PIN, INTERRUPT_PIN);

// 20 min = 20 min * 60 s * 1000 ms = 1'200'000 ms
unsigned long lastSendingTime = SENDING_PERIOD;
unsigned long lastDebugLedBlink = LED_BLINKING_PERIOD;
unsigned long now;


void setup(void) {
	#if WATCHDOG_ENABLED
		wdt_disable();
		delay(2);
		wdt_reset();
		wdt_enable(WATCHDOG_RESET_TIME);
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
		readSensors();
		if (weatherData.getHumidity() != (-1)) {
			#if WATCHDOG_ENABLED
				wdt_reset();
			#endif
			manager.sendData(SINK_NODE_ADDR, weatherData, &led);
		}
	);

	wdt_reset();

	TIME_DRIVEN_EVENT(now, lastDebugLedBlink, LED_BLINKING_PERIOD,
		led.toggleLed(2, 22);
	);

	manager.maintainRouting();

	#if DEBUG_ENABLED
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
