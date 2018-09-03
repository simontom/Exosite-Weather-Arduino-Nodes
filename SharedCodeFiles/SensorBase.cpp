
#include "SensorBase.h"


boolean SensorBase::readSensors(WeatherData &data) {
	boolean isSuccessful = false;

	#if DEBUG_ENABLED
	Serial.print(F("\nsensing_"));
	Serial.println(millis());
	#endif

	isSuccessful = readDataFromSensors(data);

	#if DEBUG_ENABLED
	printWeatherData(data);
	Serial.print(F("sensed_"));
	Serial.println(millis());
	#endif

	return isSuccessful;
}
