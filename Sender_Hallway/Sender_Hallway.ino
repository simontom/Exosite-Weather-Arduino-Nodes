
#include "Radio.h"
#include "Settings.h"
#include "Utilities.h"
#include "WeatherData.h"

#include <BMP180.h>

#include <LEDutilities.h>


// Hardware configuration
////////
#define LED_PIN 9

WeatherData weatherData;
Radio manager(HALLWAY_NODE_ADDR, SLAVE_SELECT_PIN, INTERRUPT_PIN);
BMP180 bmp180;
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
	
	initBMP180();
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
		if (weatherData.getPressure() != (-1)) {
			manager.sendWeatherData(SINK_NODE_ADDR, weatherData, &led);
		}
	);

	TIME_DRIVEN_EVENT(now, lastLedBlinkTime, LED_BLINKING_PERIOD,
		led.toggleLed(2, 28);
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
