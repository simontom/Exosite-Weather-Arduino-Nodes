
#if SINK_ENABLE_SENSOR

void readLocalDHT22(void) {
	uint8_t iter = 0;
	do {
		delay(2);
		dht22.doReadings();
	} while (!dht22.isDataCorrect() && (iter++ < 11));

	weatherData.temperature = (dht22.isDataCorrect() ? dht22.getTemperature() : (-1));
	weatherData.humidity = (dht22.isDataCorrect() ? dht22.getHumidity() : (-1));
}

#endif
