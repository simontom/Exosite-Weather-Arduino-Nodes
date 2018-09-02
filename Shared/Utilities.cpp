#include "Utilities.h"


void rebootDevice(void) {
	wdt_enable(WDTO_2S);
	for (; ; ) { ; }
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