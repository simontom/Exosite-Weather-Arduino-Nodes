
#include "SensorDht22Interier.h"


SensorDht22Interier::SensorDht22Interier(Radio &manager, uint8_t sensorDataPin, bool is8Mhz) : manager(manager) {
	dht22 = new DHT_LITE(sensorDataPin, is8Mhz, DHT22);
}

bool SensorDht22Interier::readDataFromSensors(WeatherData &data) {
	uint8_t iter = 0;

	for (;;) {
		dht22->doReadings();
		#if WATCHDOG_ENABLED
			wdt_reset();
		#endif
		delay(DELAY_BETWEEN_DHT22_READINGS_MS);
		manager.maintainRouting();

		if (dht22->isDataCorrect() || (++iter >= MAX_DHT22_TRIED_READINGS)) {
			break;
		}
	};

	data.setTemperature(dht22->getTemperature());
	data.setHumidity(dht22->getHumidity());

	return dht22->isDataCorrect();
}
