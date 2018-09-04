
#include "Radio.h"
#include "Settings.h"
#include "Utilities.h"
#include "WeatherData.h"

#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#include <SPI.h>
#include <I2C_Rev5\I2C.h>
#include <LEDutilities.h>
#include <BH1750FVI.h>
#include <UVindex.h>
#include <RH_RF22.h>
#include <RHMesh.h>


#define NODE_ADDR LIGHT_NODE_ADDR


// Pins
#define LED_RED_PIN 5
#define LED_GREEN_PIN 9
#define VOLTAGE_PIN A3
#define UV_INDEX_SENSOR_SUPPLY_PIN A0
#define LIGHTSENSOR_ADDR_PIN A2
#define UVINDEX_PIN A1

// Sleeping
uint16_t sleep_cycles_remaining;

// Sensors
LEDutilities ledG(LED_GREEN_PIN);
LEDutilities ledR(LED_RED_PIN);
BH1750FVI LightSensor;
UVindex uvIndex(UVINDEX_PIN);
Radio manager(NODE_ADDR, SLAVE_SELECT_PIN, INTERRUPT_PIN);

#define DEBUG_WITH_LED_GREEN(body)	DEBUG_WITH_LED(ledG, body)
#define DEBUG_WITH_LED_RED(body)	DEBUG_WITH_LED(ledR, body)


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
