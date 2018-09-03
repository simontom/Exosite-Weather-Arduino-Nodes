
inline void uvIndexSensorOn(void) {
	digitalWrite(UV_INDEX_SENSOR_SUPPLY_PIN, HIGH);
	pinMode(UV_INDEX_SENSOR_SUPPLY_PIN, OUTPUT);
	delay(22);
}

inline void uvIndexSensorOff(void) {
	pinMode(UV_INDEX_SENSOR_SUPPLY_PIN, INPUT);
	digitalWrite(UV_INDEX_SENSOR_SUPPLY_PIN, LOW);
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
	#if DEBUG_ENABLED
		Serial.println(F("sensing"));
	#endif
	
	readUVindex();
	readLightIntensity();
	
	// Read battery voltage
	weatherData.volt_mv = getVoltage_mV();

	#if DEBUG_ENABLED
		Serial.println(weatherData.temperature);
		Serial.println(weatherData.humidity);
		Serial.println(weatherData.pressure); // Velocity
		Serial.println(weatherData.volt_mv);

		Serial.println(F("sensed"));
	#endif
}
