#ifndef _SENSOR_DHT_22_h
#define _SENSOR_DHT_22_h


#include "SensorBase.h"
#include "Utilities.h"
#include "WeatherData.h"
#include "Settings.h"
#include <Arduino.h>
#include <DHT_LITE.h>


class SensorDht22 : public SensorBase {
	
	public:
		SensorDht22(void);
		virtual boolean readSensors(WeatherData &data);

};

#endif
