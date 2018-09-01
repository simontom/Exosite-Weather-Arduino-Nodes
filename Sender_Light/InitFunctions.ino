
void initRadio(void) {
	#if DEBUG_ENABLED && true
		Serial.print(F("Radio: "));
	#endif
	if ( !manager.init() ) {
		#if DEBUG_ENABLED && true
				Serial.println(F("ERROR"));
		#endif

		STOP_HERE_BLINK();
		//canSendData = false;
	}
	else {
		#if DEBUG_ENABLED && true
				Serial.println(F("OK"));
		#endif

		manager.setTimeout(TIMEOUT);
		manager.setRetries(RETRIES);

		// Enables to make various networks (some kind of PAN ID)
		driver.setSyncWords(SYNC_WORDS, 2);
		
		// Defaults after init are 434.0MHz, 0.05MHz AFC pull-in
		driver.setModemConfig(RADIO_MODEM_CONFIG);
		driver.setTxPower(RADIO_POWER_LOWER);

		#if !USE_MESH_LIBRARY
			manager.addRouteTo(SINK_NODE_ADDR, VERANDA_NODE_ADDR);
			//manager.addRouteTo(SINK_NODE_ADDR, LIVROOM_NODE_ADDR);
			//manager.addRouteTo(BATHROOM_NODE_ADDR, VERANDA_NODE_ADDR);
			//manager.addRouteTo(LIVROOM_NODE_ADDR, VERANDA_NODE_ADDR);
			//manager.addRouteTo(VERANDA_NODE_ADDR, VERANDA_NODE_ADDR);
			//manager.addRouteTo(LIVROOM2_NODE_ADDR, VERANDA_NODE_ADDR);
			//manager.addRouteTo(LIGHT_NODE_ADDR, VERANDA_NODE_ADDR);
		#endif
	}
}
