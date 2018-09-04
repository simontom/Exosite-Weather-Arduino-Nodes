
// INCLUDES
////////
#include "Settings.h"
#include "WeatherData.h"
#include "Radio.h"
#include "Utilities.h"
#if SINK_ENABLE_SENSOR
#include "SensorDht22Interier.h"
#endif
#include <LEDutilities.h>


// Hardware configuration
////////
#define LED_PIN 9
#define DHT22_DATA_PIN A2

LEDutilities led(LED_PIN);
WeatherData weatherData;
Radio manager(SINK_NODE_ADDR, SLAVE_SELECT_PIN, INTERRUPT_PIN);

#if SINK_ENABLE_SENSOR
SensorDht22Interier sensor(manager, DHT22_DATA_PIN);
unsigned long lastSendingTime = SENDING_PERIOD;
#endif
unsigned long lastDebugLedBlink = LED_BLINKING_PERIOD_RECEIVER;
unsigned long now;


void setup(void) {
	#if WATCHDOG_ENABLED
	initializeWdtOnStartup();
	#endif

	Serial.begin(57600);
	manager.init(led);
	led.blinkLed(3, 222);
}

void loop(void) {
	#if WATCHDOG_ENABLED
	wdt_reset();
	#endif

	manager.receiveWeatherDataAndProcess(weatherData, &sendDataToRasPi);

	//handleSerialData();

	now = millis();

	#if SINK_ENABLE_SENSOR
	TIME_DRIVEN_EVENT(now, lastSendingTime, SENDING_PERIOD,
		sendDataToRasPi(BEDROOM_NODE_ADDR);
	);
	#endif

	TIME_DRIVEN_EVENT(now, lastDebugLedBlink, LED_BLINKING_PERIOD_RECEIVER,
		led.blinkLed(1, 17);
	);
}
