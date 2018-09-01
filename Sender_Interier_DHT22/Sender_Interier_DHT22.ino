
// INCLUDES
////////////
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

// Pins
#define ledPin 9
#define dht22DataPin A3

// Sensors
DHT dht22(dht22DataPin, DHT22);
LEDutilities led(ledPin);

// 20 min = 20 min * 60 s * 1000 ms = 1'200'000 ms
unsigned long lastSendingTime = SENDING_PERIOD;
unsigned long lastDebugLedBlink = DEBUG_BLINKING_PERIOD;
unsigned long now;

// Wireless Transceivers
//#define NODE_ADDR	BEDROOM_NODE_ADDR
//#define NODE_ADDR	BATHROOM_NODE_ADDR
#define NODE_ADDR	LIVROOM_NODE_ADDR

RH_RF22 driver(3, 2);
#if USE_MESH_LIBRARY
RHMesh manager(driver, NODE_ADDR);
#else
RHRouter manager(driver, BEDROOM_NODE_ADDR);
#endif

// Watchdog
#define WATCHDOG_ENABLED true
#if WATCHDOG_ENABLED
uint8_t tx_errors_counter = 0;
#endif


void setup(void) {
	#if WATCHDOG_ENABLED
		wdt_disable();
		delay(2);
		wdt_reset();
		wdt_enable(WATCHDOG_RESET_TIME);
	#endif

	#if DEBUG_ENABLED && true
		Serial.begin(57600);
		printFreeRam(); ////
	#endif
	
	initRadio();

	#if WATCHDOG_ENABLED
		wdt_reset();
	#endif

	led.blinkLed(3, 222);

	//printFreeRam(); ////
}


void loop(void) {
	#if WATCHDOG_ENABLED
		wdt_reset();
	#endif

	now = millis();

	TIME_DRIVEN_EVENT(now, lastSendingTime, SENDING_PERIOD,
		readSensors();
		wdt_reset();
		if (weatherData.humidity != (-1)) {
			wdt_reset();
			sendData();
		}
	);

	wdt_reset();

	TIME_DRIVEN_EVENT(now, lastDebugLedBlink, DEBUG_BLINKING_PERIOD,
		led.toggleLed(2, 22);
	);

	maintainRouting();

	#if DEBUG_ENABLED && true
		if (Serial.available()) {
			if (Serial.read() == 'i') {
				manager.printRoutingTable();
				Serial.print(F("RSSI: "));
				Serial.println(driver.lastRssi());
				printFreeRam(); ////
				Serial.println();
			}
			Serial.flush();
		}
	#endif
}
