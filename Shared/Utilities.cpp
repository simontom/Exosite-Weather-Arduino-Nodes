#include "Utilities.h"


#if WATCHDOG_ENABLED
void initializeWdtOnStartup(void) {
	// Disables WDT if it is the reason device is starting-up
	wdt_disable();
	delay(2);
	wdt_reset();
	wdt_enable(WATCHDOG_RESET_TIME);
}
#endif

// 200 kOhm from Vcc to GND - drain around 21 uA
uint16_t getVoltage_mV(uint8_t voltagePin) {

	// Set 1V1 reference
	analogReference(INTERNAL);
	delayMicroseconds(2);

	// Dump first reading
	analogRead(voltagePin);

	float battVolts = 0;
	for (byte i = 0; i < 8; i++) {
		battVolts += analogRead(voltagePin);
	}
	battVolts /= 8.0;

	// ADCval * Vref / ADC's resolution
	// This (Vref / ADC's resolution) is called LSB
	battVolts *= (1100.0 / 1023.0);

	// ADCval * (RtoGND + R2toVcc) / RtoGND
	battVolts *= (199.0 / 51.3);

	return battVolts;
}

void rebootDevice(void) {
	#if DEBUG_ENABLED
		Serial.println(F(".... REBOOTING DEVICE ...."));
	#endif
	wdt_enable(WATCHDOG_REBOOT_DEVICE_TIME);
	for (; ; ) { ; }
}

#if DEBUG_ENABLED
void printWeatherData(WeatherData &data) {
	Serial.print(weatherData.getTemperature());
	Serial.println(F("*C"));
	Serial.print(weatherData.getHumidity());
	Serial.println(F("%"));
	Serial.print(weatherData.getMilivolts());
	Serial.println(F("mV"));
	Serial.print(weatherData.getPressure());
	Serial.println(F("%"));
}

int getFreeRam(void) {
	extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);

}
void printFreeRam(void) {
	Serial.print(F("SRAM: "));
	Serial.println(getFreeRam());
	delay(11);
}
#endif
