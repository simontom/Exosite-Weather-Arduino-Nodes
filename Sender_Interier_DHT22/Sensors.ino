
void readSensors(void) {
	#if DEBUG_ENABLED
		Serial.println(F("\nsensing"));
	#endif

	do {
		delay(10);
		dht22.doReadings();
		#if WATCHDOG_ENABLED
			wdt_reset();
		#endif
		manager.maintainRouting();
	} while (!dht22.isDataCorrect());

	weatherData.setTemperature(dht22.isDataCorrect() ? dht22.getTemperature() : (-1));
	weatherData.setHumidity(dht22.isDataCorrect() ? dht22.getHumidity() : (-1));

	#if DEBUG_ENABLED
		Serial.println(weatherData.getTemperature());
		Serial.println(weatherData.getHumidity());
		Serial.println(F("sensed"));
	#endif
}

