
inline void uvIndexSensorOn(void) {
	digitalWrite(uvIndexSensorSupplyPin, HIGH);
	pinMode(uvIndexSensorSupplyPin, OUTPUT);
	delay(22);
}

inline void uvIndexSensorOff(void) {
	pinMode(uvIndexSensorSupplyPin, INPUT);
	digitalWrite(uvIndexSensorSupplyPin, LOW);
}


// 200 kOhm from Vcc to GND - drain around 21 uA
uint16_t getVoltage_mV(void) {

	// Set 1V1 reference
	analogReference(INTERNAL);
	delayMicroseconds(2);

	// Dump first reading
	analogRead(voltagePin);

	float battVolts = 0;
	for (byte i = 0; i < 8; i++) {
		battVolts += analogRead(voltagePin);
	}
	battVolts /= 8.0;

	// ADCval * Vref / ADC's resolution
	// This (Vref / ADC's resolution) is called LSB
	battVolts *= (1100.0 / 1023.0);

	// ADCval * (RtoGND + R2toVcc) / RtoGND
	battVolts *= (199.0 / 51.3);

	return battVolts;

}


void readLightIntensity(void) {
	LightSensor.WakeUp();
	weatherData.temperature = LightSensor.GetLightIntensity();
	LightSensor.PowerDown();
}


void readUVindex(void) {
	uvIndexSensorOn();
	uvIndex.doReadings();
	weatherData.humidity = uvIndex.getUVindex();
	uvIndexSensorOff();
}


void readSensors(void) {
	#if DEBUG_ENABLED && true
		Serial.println(F("sensing"));
	#endif
	
	readUVindex();
	readLightIntensity();
	
	// Read battery voltage
	weatherData.volt_mv = getVoltage_mV();

	#if DEBUG_ENABLED && true
		Serial.println(weatherData.temperature);
		Serial.println(weatherData.humidity);
		Serial.println(weatherData.pressure); // Velocity
		Serial.println(weatherData.volt_mv);

		Serial.println(F("sensed"));
	#endif
}
