
void initRadio(void) {
	if ( !manager.init() ) {
		Serial.println(F("ERROR"));
		STOP_HERE_LED(led);
	}
	else {
		Serial.println(F("OK"));

		manager.setTimeout(TIMEOUT);
		manager.setRetries(RETRIES);

		// Enables to make various networks (some kind of PAN ID)
		driver.setSyncWords(SYNC_WORDS, 2);

		// Defaults after init are 434.0MHz, 0.05MHz AFC pull-in
		driver.setModemConfig(RADIO_MODEM_CONFIG);
		driver.setTxPower(RADIO_POWER_LOWER);

		#if !USE_MESH_LIBRARY
			manager.addRouteTo(OUTSIDE_NODE_ADDR, LIVROOM_NODE_ADDR);
			manager.addRouteTo(BATHROOM_NODE_ADDR, BATHROOM_NODE_ADDR);
			manager.addRouteTo(LIVROOM_NODE_ADDR, LIVROOM_NODE_ADDR);
			manager.addRouteTo(VERANDA_NODE_ADDR, LIVROOM_NODE_ADDR);
			manager.addRouteTo(LIVROOM2_NODE_ADDR, LIVROOM_NODE_ADDR);
			manager.addRouteTo(LIGHT_NODE_ADDR, LIVROOM_NODE_ADDR);
			manager.addRouteTo(BEDROOM2_NODE_ADDR, LIVROOM_NODE_ADDR);
		#endif
	}
}