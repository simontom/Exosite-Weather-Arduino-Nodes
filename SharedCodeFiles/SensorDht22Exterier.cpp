
#include "SensorDht22Exterier.h"


SensorDht22Exterier::SensorDht22Exterier(Radio &manager, uint8_t sensorDataPin, uint8_t sensorSupplyPin, bool is8Mhz)
	: SensorDht22Interier(manager, sensorDataPin, is8Mhz), sensorSupplyPin(sensorSupplyPin) {

	dht22 = new DHT_LITE(sensorDataPin, is8Mhz, DHT22);
	liFuelGauge = new LiFuelGauge(MAX17043);
	//liFuelGauge->reset();
}

bool SensorDht22Exterier::readDataFromSensors(WeatherData &data) {
	bool isDataCorrect = false;
	sensorOn();
	isDataCorrect = SensorDht22Interier::readDataFromSensors(data);
	sensorOff();

	data.setMilivolts((uint16_t)(liFuelGauge->getVoltage() * 1000));
	data.setPressure((float)liFuelGauge->getStateOfCharge());

	return isDataCorrect;
}

// Sensor Supply logic
///////////////////////
void SensorDht22Exterier::setSensorSupplyState(bool turnOn) {
	pinMode(sensorSupplyPin, (turnOn ? OUTPUT : INPUT));
	digitalWrite(sensorSupplyPin, (turnOn ? HIGH : LOW));
}

void SensorDht22Exterier::sensorOn(void) {
	setSensorSupplyState(true);
	delay(22);
}

void SensorDht22Exterier::sensorOff(void) {
	setSensorSupplyState(false);
}
