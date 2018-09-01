
inline void turnSupplyDhtOn(void) {
	digitalWrite(PIN_SUPPLY_DHT22, HIGH);
	pinMode(PIN_SUPPLY_DHT22, OUTPUT);
	delay(8);
}

inline void dhtSupplyDhtOff(void) {
	pinMode(PIN_SUPPLY_DHT22, INPUT);
	digitalWrite(PIN_SUPPLY_DHT22, LOW);
}


// 200 kOhm from Vcc to GND - drain around 21 uA
/*float getVoltage_mV(void) {
	
	// Set 1V1 reference
	analogReference(INTERNAL);
	delayMicroseconds(2);

	// Dump first reading
	analogRead(PIN_ANALOG_VOLTAGE);

	float battVolts = 0;
	for (byte i = 0; i < 8; i++) {
		battVolts += analogRead(PIN_ANALOG_VOLTAGE);
	}
	battVolts /= 8.0;
	
	// ADCval * Vref / ADC's resolution
	// This (Vref / ADC's resolution) is called LSB
	battVolts *= (1100.0 / 1023.0);

	// ADCval * ((RtoGND + R2toVcc) / RtoGND)
	battVolts *= ((196.7) / 46.7);

	return ( battVolts );
}*/


void readDhtSensor(void) {
	uint8_t iter = 0;
	int state = DHTLIB_ERROR_CHECKSUM;

	turnSupplyDhtOn();
	for (;;) {
		state = dht.read22(PIN_DATA_DHT_22);
		#if DEBUG_ENABLED && true
			Serial.print(F("Reading state: ")); Serial.println(state);
		#endif
		if ((state == DHTLIB_OK) || (++iter >= MAX_DHT22_TRIED_READINGS)) {
			break;
		}
		delay(DELAY_BETWEEN_DHT22_READINGS_MS);
	}
	dhtSupplyDhtOff();

	weatherData.temperature = (state == DHTLIB_OK) ? dht.temperature : (-1);
	weatherData.humidity = (state == DHTLIB_OK) ? dht.humidity : (-1);
}


// Return TRUE if data is correct
bool readSensors(void) {
	#if DEBUG_ENABLED && true
		Serial.print(F("sensing_"));
		Serial.println(millis());
	#endif
	
	readDhtSensor();
	
	// Read battery voltage
	weatherData.volt_mv = (uint16_t)(liFuelGauge.getVoltage() * 1000);
	weatherData.pressure = (float)liFuelGauge.getStateOfCharge();

	#if DEBUG_ENABLED && true
		Serial.print(weatherData.temperature);
		Serial.println(F("*C"));
		Serial.print(weatherData.humidity);
		Serial.println(F("%"));
		Serial.print(weatherData.volt_mv);
		Serial.println(F("mV"));
		Serial.print(weatherData.pressure);
		Serial.println(F("%"));

		Serial.print(F("sensed_"));
		Serial.println(millis());
	#endif

	return (weatherData.humidity != (-1));
}
