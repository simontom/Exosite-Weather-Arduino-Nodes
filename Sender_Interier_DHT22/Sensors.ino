
void readSensors(void) {
	#if DEBUG_ENABLED && true
		Serial.println(F("\nsensing"));
	#endif

	do {
		delay(10);
		dht22.doReadings();
		#if WATCHDOG_ENABLED
			wdt_reset();
		#endif
		maintainRouting();
	} while (!dht22.isDataCorrect());

	weatherData.temperature = (dht22.isDataCorrect() ? dht22.getTemperature() : (-1));
	weatherData.humidity = (dht22.isDataCorrect() ? dht22.getHumidity() : (-1));

	#if DEBUG_ENABLED && true
		Serial.println(weatherData.temperature);
		Serial.println(weatherData.humidity);
		Serial.println(F("sensed"));
	#endif
}

