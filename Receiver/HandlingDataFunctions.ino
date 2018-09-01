
void processOutside(void) {
	Serial.print(F("temOut="));
	Serial.print(weatherData.temperature);
	Serial.print(F("&humOut="));
	Serial.print(weatherData.humidity);
	Serial.print(F("&socOut=")); // State Of Charge [%]
	Serial.print(weatherData.pressure);
	Serial.print(F("&volOut="));
	Serial.println(weatherData.volt_mv);
}

void processBathroom(void) {
	Serial.print(F("temBat="));
	Serial.print(weatherData.temperature);
	Serial.print(F("&humBat="));
	Serial.println(weatherData.humidity);
}

void processBedroom(void) {
	Serial.print(F("temBed="));
	Serial.print(weatherData.temperature);
	Serial.print(F("&humBed="));
	Serial.println(weatherData.humidity);
}

void processBedroom2(void) {
	Serial.print(F("temBed2="));
	Serial.print(weatherData.temperature);
	Serial.print(F("&humBed2="));
	Serial.println(weatherData.humidity);
}

void processLivRoom(void) {
	Serial.print(F("temLiv="));
	Serial.print(weatherData.temperature);
	Serial.print(F("&humLiv="));
	Serial.println(weatherData.humidity);
}

void processLivRoom2(void) {
	Serial.print(F("temLiv2="));
	Serial.print(weatherData.temperature);
	Serial.print(F("&humLiv2="));
	Serial.println(weatherData.humidity);
}

void processHallway(void) {
	Serial.print(F("preHal="));
	Serial.print(weatherData.pressure);
	Serial.print(F("&temHal="));
	Serial.println(weatherData.temperature);
}

void processLight(void) {
	Serial.print(F("ligOut2="));
	Serial.print(weatherData.temperature);
	Serial.print(F("&uvOut2="));
	Serial.print((int)weatherData.humidity);
	Serial.print(F("&volOut2="));
	Serial.println(weatherData.volt_mv);
}

void processUnknownAddr(uint8_t from) {
	Serial.print(F("Warning: Unknown Address "));
	Serial.println(from, DEC);
}


void sendDataToRasPi(uint8_t fromNode) {
	switch (fromNode) {
		case OUTSIDE_NODE_ADDR:
			processOutside();
			break;
		case BATHROOM_NODE_ADDR:
			processBathroom();
			break;
		case BEDROOM_NODE_ADDR:
			#if SINK_ENABLE_SENSOR
				readLocalDHT22();
			#endif
			if ((weatherData.temperature != (-1)) || (weatherData.humidity != (-1))) {
				processBedroom();
			}
			break;
		case BEDROOM2_NODE_ADDR:
			processBedroom2();
			break;
		case LIVROOM_NODE_ADDR:
			processLivRoom();
			break;
		case LIVROOM2_NODE_ADDR:
			processLivRoom2();
			break;
		case HALLWAY_NODE_ADDR:
			processHallway();
			break;
		case LIGHT_NODE_ADDR:
			processLight();
			break;
		default:
			processUnknownAddr(fromNode);
	}
}


void processRadioData(void) {
	uint8_t len = weatherDataLen;

	if (manager.recvfromAck(weatherDataPtr, &len, &from)) {

		#if DEBUG_ENABLED && true
			Serial.print(F("\nfrom 0x"));
			Serial.print(from, HEX);
			Serial.print(F(" at: "));
			Serial.println( millis() );
		#endif

		sendDataToRasPi(from);
				
		#if DEBUG_ENABLED && true
			Serial.print(F("RSSI: "));
			Serial.println( driver.lastRssi() );
		#endif
			
	}

}


//void handleSerialData(void) {
//	if (Serial.available() > 0) {
//		uint8_t data[4];
//		int tmp;
//		for (uint8_t i = 0; i < 4; i++) {
//			tmp = Serial.parseInt();
//			data[i] = tmp & 0xFF;
//		}
//		manager.sendtoWait(data, 4, RGB_NODE_ADDR);
//	}
//}
