
void initBMP180(void) {
	#if DEBUG_ENABLED && true
		Serial.print(F("BMP180: "));
	#endif

	if (!bmp180.begin()) {
		#if DEBUG_ENABLED && true
				Serial.println(F("ERROR"));
		#endif

		STOP_HERE_LED(led);
	}
	else {
		#if DEBUG_ENABLED && true
				Serial.println(F("OK"));
		#endif
	}
}
