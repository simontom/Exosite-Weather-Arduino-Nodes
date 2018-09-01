
// INCLUDES
////////////
#include <SPI.h>
#include <I2C_Rev5\I2C.h>
#include <BMP180.h>
#include <LEDutilities.h>
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

// Sensors
BMP180 bmp180;

LEDutilities led(ledPin);

unsigned long lastSendingTime = SENDING_PERIOD;
unsigned long lastDebugLedBlink = DEBUG_BLINKING_PERIOD;
unsigned long now;

// Wireless Transceivers
RH_RF22 driver(3, 2);
#if USE_MESH_LIBRARY
RHMesh manager(driver, HALLWAY_NODE_ADDR);
#else
RHRouter manager(driver, VERANDA_NODE_ADDR);
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
	
	initBMP180();

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
		if (weatherData.pressure != (-1)) {
			sendData();
		}
	);

	TIME_DRIVEN_EVENT(now, lastDebugLedBlink, DEBUG_BLINKING_PERIOD,
		led.toggleLed(2, 28);
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
