
#include "SensorBase.h"


SensorBase::SensorBase(void) {
}

boolean SensorBase::readSensors(WeatherData &data)
{
	boolean isSuccessful = false;

	#if DEBUG_ENABLED
		Serial.println(F("\nsensing"));
	#endif

	isSuccessful = readDataFromSensors(data);

	#if DEBUG_ENABLED
			printWeatherData(data);
			Serial.print(F("sensed_"));
			Serial.println(millis());
	#endif

	return isSuccessful;
}
