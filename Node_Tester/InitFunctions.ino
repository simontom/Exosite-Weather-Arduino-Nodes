
void initRadio(void) {
	Serial.print(F("Radio: "));
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
	}
}
