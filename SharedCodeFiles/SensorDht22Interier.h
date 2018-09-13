#ifndef _SENSOR_DHT_22_INTERIER_h
#define _SENSOR_DHT_22_INTERIER_h


#include "SensorBase.h"
#include "Utilities.h"
#include "WeatherData.h"
#include "Settings.h"
#include "Radio.h"
#include <DHT_LITE.h>

#if WATCHDOG_ENABLED
#include <avr/wdt.h>
#endif


class SensorDht22Interier : public SensorBase {

	public:
		SensorDht22Interier(Radio &manager, uint8_t sensorDataPin, bool is8Mhz = false);

	protected:
		DHT_LITE *dht22;
		Radio &manager;

		virtual bool readDataFromSensors(WeatherData &data);

};

#endif
