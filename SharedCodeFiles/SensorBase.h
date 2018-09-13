#ifndef _SENSOR_BASE_h
#define _SENSOR_BASE_h


#include "Utilities.h"
#include "WeatherData.h"
#include "Settings.h"
#include <Arduino.h>


class SensorBase {
	
	public:
		bool readSensors(WeatherData &data);

	protected:
		virtual bool readDataFromSensors(WeatherData &data) = 0;

};

#endif
