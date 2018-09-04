#include "Radio.h"


Radio::Radio(uint8_t address, uint8_t slaveSelectPin, uint8_t interruptPin) {
	driver = new RH_RF22(slaveSelectPin, interruptPin);
	manager = new RHMesh(*driver, address);
}

void Radio::init(LEDutilities &led) {
	if (!(manager->init())) {
		#if DEBUG_ENABLED
		Serial.println(F("Radio: ERROR"));
		#endif

		PROTECT_WITH_WDT(
			STOP_HERE_LED(led);
		);
	}
	else {
		#if DEBUG_ENABLED
		Serial.println(F("Radio: OK"));
		#endif

		// Enables to make various networks (some kind of PAN ID)
		driver->setSyncWords(SYNC_WORDS, 2);
		// Defaults after init are 434.0MHz, 0.05MHz AFC pull-in
		driver->setModemConfig(RADIO_MODEM_CONFIG);
		driver->setTxPower(RADIO_POWER_LOWER);

		manager->setTimeout(TIMEOUT);
		manager->setRetries(RETRIES);
	}

	#if DEBUG_ENABLED
	Serial.println(F("initRadio end"));
	printFreeRam();
	#endif
}

void Radio::maintainRouting(void) {
	// Call "recvfromAck" to do all the routing/mesh discovery/internal magic
	uint8_t length = buffLen;
	if (manager->recvfromAck(buff, &length, &from)) {
		#if DEBUG_ENABLED
		Serial.print(F("\nfrom 0x"));
		Serial.print(from, HEX);
		Serial.print(F(" dataContainer length "));
		Serial.print(length);
		Serial.print(F(" _ "));
		Serial.println(millis());
		#endif
		manager->sendtoWait(buff, length, from);
	}
}

void Radio::receiveWeatherDataAndProcess(WeatherData &dataContainer, WeatherDataProcessor dataProcessor) {
	uint8_t dataLen = dataContainer.getDataLength();
	if (manager->recvfromAck(dataContainer.getDataPointer(), &dataLen, &from)) {
		#if DEBUG_ENABLED
		Serial.print(F("\nfrom 0x"));
		Serial.print(from, HEX);
		Serial.print(F(" at: "));
		Serial.println(millis());
		#endif

		dataProcessor(from);

		#if DEBUG_ENABLED
		Serial.print(F("RSSI: "));
		Serial.println(driver.lastRssi());
		#endif
	}
}

void Radio::receiveDataAndProcessWithTimeout(uint8_t *buffer, uint8_t length, uint16_t timeout, DataProcessor dataProcessor) {
	boolean isReceivedSuccessfully = manager->recvfromAckTimeout(buffer, &length, timeout, &from);
	dataProcessor(isReceivedSuccessfully, from, length, buffer);
}

boolean Radio::sendWeatherData(uint8_t destinationAddress, WeatherData &data, LEDutilities *led) {
	return sendData(destinationAddress, data.getDataPointer(), data.getDataLength(), led);
}

boolean Radio::sendData(uint8_t destinationAddress, uint8_t *data, uint8_t length, LEDutilities *led) {
	#if DEBUG_ENABLED
	Serial.print(F("\nsending_"));
	Serial.println(millis());
	printFreeRam();
	#endif

	uint8_t error = manager->sendtoWait(data, length, destinationAddress);
	boolean isSentSuccessfully = error == RH_ROUTER_ERROR_NONE;

	#if DEBUG_ENABLED
	switch (error) {
		case RH_ROUTER_ERROR_NONE:
			Serial.println(F("Sent to next hop"));
			break;
		case RH_ROUTER_ERROR_NO_ROUTE:
			Serial.println(F("No route in routing table"));
			break;
		case RH_ROUTER_ERROR_UNABLE_TO_DELIVER:
			Serial.println(F("Unable to deliver to next hop"));
			break;
	}
	#endif

	if (isSentSuccessfully) {
		if (led != nullptr) {
			led->offLed();
		}

		#if WATCHDOG_ENABLED
		tx_errors_counter = 0;
		#endif
	}
	else {
		if (led != nullptr) {
			led->onLed();
		}

		#if WATCHDOG_ENABLED
		tx_errors_counter++;
		#if DEBUG_ENABLED
		Serial.print(F("TX_ERROR_COUNTER "));
		Serial.println(tx_errors_counter);
		#endif
		if (tx_errors_counter >= MAX_TX_ERRORS_BEFORE_RESET) {
			rebootDevice();
		}
		#endif
	}

	#if DEBUG_ENABLED
	manager->printRoutingTable();
	Serial.print(F("RSSI: "));
	Serial.println(driver->lastRssi());
	printFreeRam(); ////
	Serial.print(F("sent_"));
	Serial.println(millis());
	Serial.println(F("\n"));
	delay(111);
	#endif

	return isSentSuccessfully;
}

void Radio::printRoutingTable(void) {
	manager->printRoutingTable();
}

int16_t Radio::getLastRssi(void) {
	return driver->lastRssi();
}
