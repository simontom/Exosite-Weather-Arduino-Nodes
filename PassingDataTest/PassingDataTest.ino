
#include "WeatherData.h"
#include <Streaming.h>


WeatherData subject;


void editSubjectByReference(WeatherData &data, float tmeprature, float humidity) {
	data.setTemperature(tmeprature);
	data.setHumidity(humidity);
}

void printSubject(WeatherData &data) {
	unsigned int dataAddress = (unsigned int)(&data);

	Serial << "REF_IN_FUNC: " << dataAddress << endl;
	Serial << "T: " << data.getTemperature() << endl;
	Serial << "H: " << data.getHumidity() << endl;
	Serial << "P: " << data.getPressure() << endl;
	Serial << "V: " << data.getMilivolts() << endl;
}


void setup() {
	Serial.begin(57600);
	// For Arduino Micro with built-in HW USB
	while (!Serial);

	delay(1000);

	Serial << endl << "Started" << endl;
	Serial << "Reference: " << ((unsigned int)(&subject)) << endl;
	Serial << millis() << endl;
	printSubject(subject);
	editSubjectByReference(subject, 123, 321);
	printSubject(subject);
	Serial << millis() << endl;
}


void loop() {
//	Serial.println(millis());
}
