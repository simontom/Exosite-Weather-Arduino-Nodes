
inline void turnSupplyDhtOn(void) {
	digitalWrite(SUPPLY_DHT22_PIN, HIGH);
	pinMode(SUPPLY_DHT22_PIN, OUTPUT);
	delay(8);
}

inline void turnSupplyDhtOff(void) {
	pinMode(SUPPLY_DHT22_PIN, INPUT);
	digitalWrite(SUPPLY_DHT22_PIN, LOW);
}

void readDhtSensor(void) {
	uint8_t iter = 0;
	int state = DHTLIB_ERROR_CHECKSUM;

	turnSupplyDhtOn();
	for (;;) {
		state = dht.read22(DATA_DHT_22_PIN);
		#if DEBUG_ENABLED
			Serial.print(F("Reading state: ")); Serial.println(state);
		#endif
		if ((state == DHTLIB_OK) || (++iter >= MAX_DHT22_TRIED_READINGS)) {
			break;
		}
		delay(DELAY_BETWEEN_DHT22_READINGS_MS);
	}
	turnSupplyDhtOff();

	weatherData.setTemperature((state == DHTLIB_OK) ? dht.temperature : (-1));
	weatherData.setHumidity((state == DHTLIB_OK) ? dht.humidity : (-1));
}


// Return TRUE if data is correct
boolean readSensors(void) {
	readDhtSensor();
	
	// Read battery voltage
	weatherData.setMilivolts((uint16_t)(liFuelGauge.getVoltage() * 1000));
	weatherData.setPressure((float)liFuelGauge.getStateOfCharge());

	return (weatherData.getHumidity() != (-1));
}
