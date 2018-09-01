
void sendData(void) {
	#if DEBUG_ENABLED && true
		Serial.println(F("sending"));
	#endif

	//wdt_enable(WATCHDOG_RESET_TIME);
	//wdt_reset();
	////initRadio();
	//wdt_disable();

	uint8_t error;
	//DEBUG_WITH_LED_RED(
		error = manager.sendtoWait(weatherDataPtr, weatherDataLen, SINK_NODE_ADDR);
		#if WATCHDOG_ENABLED
			if (error != RH_ROUTER_ERROR_NONE) {
				tx_errors_counter++;
				if (tx_errors_counter >= MAX_TX_ERRORS_BEFORE_RESET) {
					rebootArduino();
				}
			}
		#endif
	//);

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

	//(error != RH_ROUTER_ERROR_NONE) ? ledR.onLed() : ledR.offLed();
	//(error != RH_ROUTER_ERROR_NONE) ? ledR.analogLed(120) : ledR.offLed();

	#if DEBUG_ENABLED && true
		manager.printRoutingTable();
		Serial.print(F("RSSI: "));
		Serial.println( driver.lastRssi() );
		Serial.println(F("sent"));
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
