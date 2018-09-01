
void printAddress(const char* name, uint8_t addr) {
	Serial.print(F("Address: "));
	Serial.print(addr, DEC);
	Serial.print(F("\tnamed: "));
	Serial.println(name);
}

void printPossibleAddresses(void) {
	printAddress("sink", SINK_NODE_ADDR);
	printAddress("bath", BATHROOM_NODE_ADDR);
	printAddress("bed", BEDROOM_NODE_ADDR);
	//printAddress("bed2", BEDROOM2_NODE_ADDR);
	printAddress("liv", LIVROOM_NODE_ADDR);
	//printAddress("liv2", LIVROOM2_NODE_ADDR);
	printAddress("hal", HALLWAY_NODE_ADDR);
	//printAddress("out", OUTSIDE_NODE_ADDR);
	//printAddress("lig", LIGHT_NODE_ADDR);
}


void printRssi(void) {
	Serial.print(F("RSSI: "));
	Serial.println(driver.lastRssi());
}


bool cmp8bitArrays(uint8_t* a1, uint8_t* a2, const uint8_t len) {
	for (uint8_t i = 0; i < len; i++) {
		if (a1[i] != a2[i]) {
			return false;
		}
	}
	return true;
}


void checkNode(uint8_t addrTo) {
	uint8_t error;
	//uint16_t helloData = 43690;	//0b1010'1010'1010'1010
	uint8_t helloData[2] = { B10101010, B01010101 };
	uint8_t rcvdData[2] = { 0, 0 };
	uint8_t len = 2;
	uint8_t from = 0xFF;
	
	Serial.print(F("Sending to: "));
	Serial.println(addrTo, DEC);

	error = manager.sendtoWait(helloData, len, addrTo);
	switch (error) {
		case RH_ROUTER_ERROR_NONE:
			Serial.println(F("Sent to next hop"));
			break;
		case RH_ROUTER_ERROR_NO_ROUTE:
			Serial.println(F("No route in routing table"));
			break;
		case RH_ROUTER_ERROR_UNABLE_TO_DELIVER:
			Serial.println(F("Unable to deliver to next hop"));
			break;
		default:
			Serial.println(F("Unknown Radio error"));
	}
	if (error != RH_ROUTER_ERROR_NONE) {
		return;
	}

	printRssi();

	// Wait for the reply and handle the response
	if (manager.recvfromAckTimeout(rcvdData, &len, 2222, &from )) {
		Serial.print(F("Reply from: "));
		Serial.print(from, DEC);
		Serial.print(F("  len: "));
		Serial.println(len, DEC);
		if (cmp8bitArrays(rcvdData, helloData, len)) {
			Serial.println(F("__Correct data"));
		}
		else {
			Serial.println(F("__Incorrect data"));
		}
		Serial.print(F("Expected: "));
		Serial.print(helloData[0], BIN);
		Serial.println(helloData[1], BIN);
		Serial.print(F("Received: "));
		Serial.print(rcvdData[0], BIN);
		Serial.println(rcvdData[1], BIN);
	}
	else {
		Serial.print(F("Node "));
		Serial.print(addrTo, DEC);
		Serial.println(F(" NOT responding"));
		if (addrTo == SINK_NODE_ADDR) {
			Serial.println(F("Check output on RasPi via VNC"));
		}
	}
}


void maintainRouting(void) {
	// Call "recvfromAck" to do all the routing/mesh discovery/internal magic
	uint8_t len = buffLen;
	manager.recvfromAck(buff, &len, &from);
	delay(4);
}
