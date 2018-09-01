
// INCLUDES
////////////
#include <SPI.h>
#include <LEDutilities.h>
#include <RH_RF22.h>
#include <_My_Project_Network_Settings.h>
#if USE_MESH_LIBRARY
#include <RHMesh.h>
#else
#include <RHRouter.h>
#endif


// Hardware configuration
////////

// Pins
#define ledPin 9

LEDutilities led(ledPin);

// Wireless Transceivers
RH_RF22 driver(3, 2);
#if USE_MESH_LIBRARY
RHMesh manager(driver, TESTING_NODE_ADDR);
#else
RHRouter manager(driver, TESTING_NODE_ADDR);
#endif


void printHelp(void) {
	//Serial.println(F("Unknown Command"));
	Serial.println(F("nXXX - checkNode"));
	Serial.println(F("a - addresses"));
	Serial.println(F("t - routingTable"));
}


void processSerialCommand(char c) {
	if (c == 'n') {
		if (Serial.available()) {
			int nodeAddr = Serial.parseInt();
			checkNode(nodeAddr);
		}
		else {
			Serial.println(F("NodeAddr not specified"));
		}
	}
	else if (c == 'a') {
		printPossibleAddresses();
	}
	else if (c == 't') {
		manager.printRoutingTable();
		printRssi();
	}
	else {
		printHelp();
	}

	Serial.flush();
	Serial.println();
}


void setup(void) {
	Serial.begin(57600);

	printPossibleAddresses();
	Serial.println();
	printHelp();
	Serial.println();
	
	initRadio();
	Serial.println();

	led.blinkLed(3, 222);
}


void loop(void) {
	if (Serial.available()) {
		char command = Serial.read();
		processSerialCommand(command);
	}

	maintainRouting();
}
