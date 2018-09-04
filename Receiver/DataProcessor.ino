
void processOutside(void) {
	Serial.print(F("temOut="));
	Serial.print(weatherData.getTemperature());
	Serial.print(F("&humOut="));
	Serial.print(weatherData.getHumidity());
	Serial.print(F("&socOut=")); // State Of Charge [%]
	Serial.print(weatherData.getStateOfCharge());
	Serial.print(F("&volOut="));
	Serial.println(weatherData.getMilivolts());
}

void processBathroom(void) {
	Serial.print(F("temBat="));
	Serial.print(weatherData.getTemperature());
	Serial.print(F("&humBat="));
	Serial.println(weatherData.getHumidity());
}

void processBedroom(void) {
	Serial.print(F("temBed="));
	Serial.print(weatherData.getTemperature());
	Serial.print(F("&humBed="));
	Serial.println(weatherData.getHumidity());
}

void processBedroom2(void) {
	Serial.print(F("temBed2="));
	Serial.print(weatherData.getTemperature());
	Serial.print(F("&humBed2="));
	Serial.println(weatherData.getHumidity());
}

void processLivRoom(void) {
	Serial.print(F("temLiv="));
	Serial.print(weatherData.getTemperature());
	Serial.print(F("&humLiv="));
	Serial.println(weatherData.getHumidity());
}

void processLivRoom2(void) {
	Serial.print(F("temLiv2="));
	Serial.print(weatherData.getTemperature());
	Serial.print(F("&humLiv2="));
	Serial.println(weatherData.getHumidity());
}

void processHallway(void) {
	Serial.print(F("preHal="));
	Serial.print(weatherData.getPressure());
	Serial.print(F("&temHal="));
	Serial.println(weatherData.getTemperature());
}

void processLight(void) {
	Serial.print(F("ligOut2="));
	Serial.print(weatherData.getTemperature());
	Serial.print(F("&uvOut2="));
	Serial.print((int)weatherData.getHumidity());
	Serial.print(F("&volOut2="));
	Serial.println(weatherData.getMilivolts());
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
			if (sensor.readSensors(weatherData)) {
				processBedroom();
			}
			#endif
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
