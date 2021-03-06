
#ifndef _WEATHERDATA_h
#define _WEATHERDATA_h


#include <Arduino.h>


class WeatherData {

	private:
	struct data_t {
		float		component1;
		float		component2;
		float		component3;
		uint16_t	component4;
	} data;
	unsigned int dataLength = sizeof(data);
	uint8_t* dataPointer = (uint8_t*)&data;

	public:
	WeatherData() {
		data.component1 = -1;
		data.component2 = -1;
		data.component3 = -1;
		data.component4 = 0;
	}

	unsigned int getDataLength(void) const {
		return dataLength;
	}
	uint8_t* getDataPointer(void) const {
		return dataPointer;
	}

	/* Using COMPONENT_1 */
	void setTemperature(float temperature) {
		data.component1 = temperature;
	}
	float getTemperature(void) const {
		return data.component1;
	}

	/* Using COMPONENT_2 */
	void setHumidity(float humidity) {
		data.component2 = humidity;
	}
	float getHumidity(void) const {
		return data.component2;
	}

	/* Using COMPONENT_3 */
	void setPressure(float pressure) {
		data.component3 = pressure;
	}
	float getPressure(void) {
		return data.component3;
	}

	/* Using COMPONENT_4 */
	void setMilivolts(uint16_t milivolts) {
		data.component4 = milivolts;
	}
	uint16_t getMilivolts(void) const {
		return data.component4;
	}

	/* Using COMPONENT_3 */
	void setStateOfCharge(float stateOfCharge) {
		data.component3 = stateOfCharge;
	}
	float getStateOfCharge(void) const {
		return data.component3;
	}

	/* Using COMPONENT_1 */
	void setLightIntensityLx(float lightIntensityLx) {
		data.component1 = lightIntensityLx;
	}
	float getLightIntensityLx(void) const {
		return data.component1;
	}

	/* Using COMPONENT_2 */
	void setUvIndex(uint8_t uvIndex) {
		data.component2 = uvIndex;
	}
	float getUvIndex(void) const {
		return data.component2;
	}

};

#endif
