#ifndef _SENSOR_DHT_22_EXTERIER_h
#define _SENSOR_DHT_22_EXTERIER_h


#include "SensorBase.h"
#include "Utilities.h"
#include "WeatherData.h"
#include "Settings.h"
#include "Radio.h"
#include "SensorDht22Interier.h"
	
#include <LiFuelGauge.h>

#if WATCHDOG_ENABLED
#include <avr/wdt.h>
#endif


class SensorDht22Exterier : public SensorDht22Interier {

	public:
		SensorDht22Exterier(Radio &manager, uint8_t sensorDataPin, uint8_t sensorSupplyPin, boolean is8Mhz = false);

	protected:
		uint8_t sensorSupplyPin;
		LiFuelGauge *liFuelGauge;

		virtual boolean readDataFromSensors(WeatherData &data);

	private:
		inline void setSensorSupplyState(boolean turnOn);
		inline void sensorOn(void);
		inline void sensorOff(void);

};

#endif
