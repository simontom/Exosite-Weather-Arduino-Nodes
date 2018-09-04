
void printHelp(void) {
	Serial.println(F("nXXX - check node"));
	Serial.println(F("a - addresses"));
	Serial.println(F("i - transceiver info"));
}

void printAddress(const char* name, uint8_t addr) {
	Serial.print(F("Address: "));
	Serial.print(addr, DEC);
	Serial.print(F("\tnamed: "));
	Serial.println(name);
}
void printAddressesList(void) {
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

void printTransceiverInfo(void) {
	manager.printRoutingTable();

	Serial.print(F("RSSI: "));
	Serial.println(manager.getLastRssi());
}

void processReceivedData(boolean isReceivedSuccessfully, uint8_t from, uint8_t length, uint8_t *data) {
	if (isReceivedSuccessfully) {
		Serial.print(F("Reply from: "));
		Serial.print(from, DEC);
		Serial.print(F("  length: "));
		Serial.println(length, DEC);

		if (cmp8bitArrays(rcvdData, helloData, length)) {
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
		Serial.print(destinationAddress, DEC);
		Serial.println(F(" NOT responding"));
		if (destinationAddress == SINK_NODE_ADDR) {
			Serial.println(F("Check output on RasPi via VNC"));
		}
	}
}
void checkNode() {
	if (Serial.available()) {
		int nodeAddr = Serial.parseInt();
		checkNode(nodeAddr);
	}
	else {
		Serial.println(F("NodeAddr not specified"));
	}
}
void checkNode(uint8_t destinationAddress) {
	boolean isSentSuccessfully;
	uint8_t length = DATA_LENGTH;
	uint8_t from = 0xFF;

	Serial.print(F("Sending to: "));
	Serial.println(destinationAddress, DEC);

	isSentSuccessfully = manager.sendData(destinationAddress, helloData, length);
	if (!isSentSuccessfully) {
		return;
	}



	// Wait for the reply and handle the response
	if (manager.recvfromAckTimeout(rcvdData, &length, 2222, &from)) {
		Serial.print(F("Reply from: "));
		Serial.print(from, DEC);
		Serial.print(F("  length: "));
		Serial.println(length, DEC);

		if (cmp8bitArrays(rcvdData, helloData, length)) {
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
		Serial.print(destinationAddress, DEC);
		Serial.println(F(" NOT responding"));
		if (destinationAddress == SINK_NODE_ADDR) {
			Serial.println(F("Check output on RasPi via VNC"));
		}
	}
}
