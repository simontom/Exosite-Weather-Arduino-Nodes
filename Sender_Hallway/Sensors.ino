
void readSensors(void) {
	#if DEBUG_ENABLED && true
		Serial.println(F("\nsensing"));
	#endif

	bmp180.readTempAndPres(&weatherData.temperature, &weatherData.pressure);

	#if DEBUG_ENABLED && true
		Serial.println(weatherData.pressure);
		Serial.println(F("sensed"));
	#endif
}
