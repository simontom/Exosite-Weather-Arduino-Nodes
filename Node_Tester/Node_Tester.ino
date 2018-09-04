
#include "Settings.h"
#include "WeatherData.h"
#include "Radio.h"
#include "Utilities.h"
#include <SPI.h>
#include <LEDutilities.h>

#include <RH_RF22.h>
#include <RHMesh.h>


#define LED_PIN 9
#define DATA_LENGTH	2


LEDutilities led(LED_PIN);
Radio manager(TESTING_NODE_ADDR, SLAVE_SELECT_PIN, INTERRUPT_PIN);

uint8_t helloData[DATA_LENGTH]	= { B10101010,	B01010101 };
uint8_t rcvdData[DATA_LENGTH]	= { 0,			0 };


void processSerialCommand(char c) {
	switch (c) {
		case 'n':
			checkNode();
			break;

		case 'a':
			printAddressesList();
			break;

		case 'i':
			printTransceiverInfo();
			break;

		default:
			printHelp();
			break;
	}

	Serial.flush();
	Serial.println();
}


void setup(void) {
	Serial.begin(57600);

	printAddressesList();
	Serial.println();
	printHelp();
	Serial.println();
	
	manager.init(led);
	Serial.println();

	led.blinkLed(3, 222);
}


void loop(void) {
	if (Serial.available()) {
		char command = Serial.read();
		processSerialCommand(command);
	}

	manager.maintainRouting();
}
