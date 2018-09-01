
// INCLUDES
////////
#include <SPI.h>
#include <LEDutilities.h>
#include <DHT.h>
#include <_My_Project_Network_Settings.h>
#include <RH_RF22.h>
#if USE_MESH_LIBRARY
	#include <RHMesh.h>
#else
	#include <RHRouter.h>
#endif


// Hardware configuration
////////
#define ledPin 9
#define dht22DataPin A2

LEDutilities led(ledPin);

// Wireless Transceivers
RH_RF22 driver(3, 2);
#if USE_MESH_LIBRARY
RHMesh manager(driver, SINK_NODE_ADDR);
#else
RHRouter manager(driver, SINK_NODE_ADDR);
#endif

// 20 min = 20 min * 60 s * 1000 ms = 1'200'000 ms
#if SINK_ENABLE_SENSOR
	DHT dht22(dht22DataPin);
	unsigned long lastSendingTime = SENDING_PERIOD;
#endif

unsigned long lastDebugLedBlink = DEBUG_BLINKING_PERIOD_RECEIVER;
unsigned long now;

// Watchdog
#define WATCHDOG_ENABLED true


void setup(void) {
	#if WATCHDOG_ENABLED
		wdt_disable();
		wdt_enable(WATCHDOG_RESET_TIME);
		wdt_reset();
	#endif

	Serial.begin(57600);
	initRadio();
	led.blinkLed(3, 222);
}

void loop(void) {
	#if WATCHDOG_ENABLED
		wdt_reset();
	#endif

	processRadioData();

	//handleSerialData();

	now = millis();

	#if SINK_ENABLE_SENSOR
		TIME_DRIVEN_EVENT(now, lastSendingTime, SENDING_PERIOD,
			sendDataToRasPi(BEDROOM_NODE_ADDR);
		);
	#endif

	TIME_DRIVEN_EVENT(now, lastDebugLedBlink, DEBUG_BLINKING_PERIOD_RECEIVER,
		led.blinkLed(1, 17);
	);

	delay(4);
}
