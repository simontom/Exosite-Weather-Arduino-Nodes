
#ifndef _WEATHERDATA_h
#define _WEATHERDATA_h

#include "arduino.h"

class WeatherData {

	private:

		struct weatherData_t {
			float component1;
			float component2;
			float component3;
			uint16_t component4;
		} data;
		unsigned int dataLength = sizeof(data);
		uint8_t* dataPointer = (uint8_t*)&data;

	public:

		WeatherData() {
			data.component1 = -1;
			data.component2 = -1;
			data.component3 = -1;
			data.component4 = -1;
		}

		unsigned int getDataLength() {
			return dataLength;
		}
		uint8_t* getDataPointer() {
			return dataPointer;
		}

		void setTemperature(float temperature) {
			data.component1 = temperature;
		}
		float getTemperature() {
			return data.component1;
		}

		void setHumidity(float humidity) {
			data.component2 = humidity;
		}
		float getHumidity() {
			return data.component2;
		}

		void setPressure(float pressure) {
			data.component3 = pressure;
		}
		float getPressure() {
			return data.component3;
		}

		void setMilivolts(uint16_t milivolts) {
			data.component4 = milivolts;
		}
		uint16_t getMilivolts() {
			return data.component4;
		}

		void setStateOfCharge(float stateOfCharge) {
			data.component3 = stateOfCharge;
		}
		float getStateOfCharge() {
			return data.component3;
		}

};

#endif
