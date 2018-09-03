
void readSensors(void) {
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
}

