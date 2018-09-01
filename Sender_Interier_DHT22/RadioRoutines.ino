
void sendData(void) {
	#if DEBUG_ENABLED && true
		Serial.println(F("sending"));
		printFreeRam();
	#endif

	uint8_t error = manager.sendtoWait(weatherDataPtr, weatherDataLen, SINK_NODE_ADDR);
	
	#if DEBUG_ENABLED && true
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

	if (error != RH_ROUTER_ERROR_NONE) {
		led.onLed();

		#if WATCHDOG_ENABLED
			tx_errors_counter++;
			#if DEBUG_ENABLED && true
				Serial.print(F("TX_ERROR_COUNTER "));
				Serial.println(tx_errors_counter);
			#endif
			if (tx_errors_counter >= MAX_TX_ERRORS_BEFORE_RESET) {
				rebootArduino();
			}
		#endif
	}
	else {
		led.offLed();

		#if WATCHDOG_ENABLED
			tx_errors_counter = 0;
		#endif
	}

	#if DEBUG_ENABLED && true
		manager.printRoutingTable();
		Serial.print(F("RSSI: "));
		Serial.println(driver.lastRssi());
		printFreeRam(); ////
		Serial.println(F("sent\n"));
		delay(111);
	#endif
}


void maintainRouting(void) {
	// Call "recvfromAck" to do all the routing/mesh discovery/internal magic
	uint8_t len = buffLen;
	if (manager.recvfromAck(buff, &len, &from)) {
		manager.sendtoWait(buff, len, from);
	}
}
