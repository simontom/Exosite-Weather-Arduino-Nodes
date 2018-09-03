#ifndef _SENSOR_BASE_h
#define _SENSOR_BASE_h


#include "Utilities.h"
#include "WeatherData.h"
#include "Settings.h"
#include <Arduino.h>


class SensorBase {
	
	public:
		SensorBase(void);
		boolean readSensors(WeatherData &data);

	protected:
		virtual boolean readDataFromSensors(WeatherData &data) = 0;

};

#endif
