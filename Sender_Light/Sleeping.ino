
void setupWatchdog(uint8_t prescalar) {
	prescalar = min(9, prescalar);
	uint8_t wdtcsr = prescalar & 7;
	if ( prescalar & 8 ) {
		wdtcsr |= _BV(WDP3);
	}
	
	MCUSR &= ~_BV(WDRF);
	WDTCSR = _BV(WDCE) | _BV(WDE);
	WDTCSR = _BV(WDCE) | wdtcsr | _BV(WDIE);
}


void fallAsleep(void) {
	//DEBUG_WITH_LED_GREEN(
		#if DEBUG_ENABLED && true
			Serial.println(F("driver.sleep\n"));
			delay(111);
		#endif
		driver.sleep();
	//);
	
	sleep_cycles_remaining = sleep_cycles_per_transmission;
	setupWatchdog(TIME_ASLEEP);
	while( sleep_cycles_remaining-- ) {
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_bod_disable();
		sleep_mode();
		
		// Wakes up here
		if (weatherData.volt_mv > 3900) { // Blink GREEN if enough Energy
			ledG.toggleLed(2, 11);
		}
		else {
			ledG.offLed();
		}
	}
	delay(2);
	wdt_disable();
}


// If I delete it, Arduino keeps restarting
// SO DO NOT DELETE THIS !!!!
ISR(WDT_vect) { ; }
