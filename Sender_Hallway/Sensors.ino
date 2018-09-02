
void readSensors(void) {
	#if DEBUG_ENABLED && true
		Serial.println(F("\nsensing"));
	#endif

	float temperature;
	float pressure;
	bmp180.readTempAndPres(&temperature, &pressure);

	weatherData.setTemperature(temperature);
	weatherData.setPressure(pressure);

	#if DEBUG_ENABLED && true
		Serial.println(weatherData.pressure);
		Serial.println(F("sensed"));
	#endif
}
