
void fallAsleep(void) {
	//DEBUG_WITH_LED_GREEN(
		#if DEBUG_ENABLED && true
			Serial.println(F("driver.sleep\n"));
			delay(88);
		#endif
		
		PROTECT_WITH_WDT(
			WATCHDOG_ENABLED,
			driver.sleep();
		);
	//);
	
	sleep_cycles_remaining = getSleepCycles();
	do {
		LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
		
		//// Wakes up here
		//if (weatherData.volt_mv >= 4000) { // Blink GREEN if enough Energy
		//	ledG.toggleLed(2, 11);
		//}
		//else {
		//	ledG.offLed();
		//}

		/*if (weatherData.humidity == (-1)) { // Blink RED if incorrect sensor data
			ledR.toggleLed(2, 11);
		}
		else {
			ledR.offLed();
		}*/
	} while (--sleep_cycles_remaining);
	delay(8);
}

inline uint16_t getSleepCycles(void) {
	if (weatherData.volt_mv >= 3855) {
		return SLEEP_CYCLES_PER_TRANSMISSION;
	}
	else if (weatherData.volt_mv >= 3650) {
		return SLEEP_CYCLES_PER_TRANSMISSION_ON_LOW_ENERGY;
	}
	else {
		return SLEEP_CYCLES_PER_TRANSMISSION_ON_TOO_LOW_ENERGY;
	}
}
