
void initBMP180(void) {
	#if DEBUG_ENABLED && true
		Serial.print(F("BMP180: "));
	#endif

	if (!bmp180.begin()) {
		#if DEBUG_ENABLED && true
				Serial.println(F("ERROR"));
		#endif

		STOP_HERE_LED(led);
	}
	else {
		#if DEBUG_ENABLED && true
				Serial.println(F("OK"));
		#endif
	}
}


void initRadio(void) {
	if ( !manager.init() ) {
		#if DEBUG_ENABLED && true
			Serial.println(F("Radio: ERROR"));
		#endif

		STOP_HERE();
	}
	else {
		#if DEBUG_ENABLED && true
			Serial.println(F("Radio: OK"));
		#endif

		manager.setTimeout(TIMEOUT);
		manager.setRetries(RETRIES);

		// Enables to make various networks (some kind of PAN ID)
		driver.setSyncWords(SYNC_WORDS, 2);

		// Defaults after init are 434.0MHz, 0.05MHz AFC pull-in
		driver.setModemConfig(RADIO_MODEM_CONFIG);
		driver.setTxPower(RADIO_POWER_LOWER);

		#if !USE_MESH_LIBRARY
			manager.addRouteTo(SINK_NODE_ADDR, LIVROOM_NODE_ADDR);
			//manager.addRouteTo(OUTSIDE_NODE_ADDR, OUTSIDE_NODE_ADDR);
			//manager.addRouteTo(BATHROOM_NODE_ADDR, BEDROOM2_NODE_ADDR);
			//manager.addRouteTo(LIVROOM_NODE_ADDR, LIVROOM_NODE_ADDR);
			//manager.addRouteTo(LIVROOM2_NODE_ADDR, LIVROOM_NODE_ADDR);
			//manager.addRouteTo(LIGHT_NODE_ADDR, LIVROOM_NODE_ADDR);
		#endif
	}
}
