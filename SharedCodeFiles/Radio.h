#ifndef _RADIO_h
#define _RADIO_h


#include "NetworkAddresses.h"
#include "Utilities.h"
#include "WeatherData.h"
#include "LEDutilities.h"
#include "Settings.h"

#include <RH_RF22.h>
#include <RHMesh.h>

#include <Arduino.h>
#include <avr/wdt.h>


// Settings
/////////////////////////////////////////////////////////
#define TIMEOUT						320
#define RETRIES						3
const uint8_t SYNC_WORDS[]	=		{ 0xD3, 0x3D };
#define RADIO_MODEM_CONFIG			RH_RF22::GFSK_Rb19_2Fd9_6
#define RADIO_POWER					RH_RF22_TXPOW_8DBM
#define RADIO_POWER_LOWER			RH_RF22_TXPOW_5DBM
#define MAX_TX_ERRORS_BEFORE_RESET	5


typedef void(*WeatherDataProcessor)(uint8_t from);
typedef void(*DataProcessor)(bool isReceivedSuccessfully, uint8_t from, uint8_t length, uint8_t *data);


class Radio {

	public:
		Radio(uint8_t address, uint8_t slaveSelectPin = SLAVE_SELECT_PIN, uint8_t interruptPin = INTERRUPT_PIN);
		void init(LEDutilities &led);

		void maintainRouting(void);
		void receiveWeatherDataAndProcess(WeatherData &dataContainer, WeatherDataProcessor dataProcessor);
		void receiveDataAndProcessWithTimeout(uint8_t *buffer, uint8_t len, uint16_t timeout, DataProcessor dataProcessor);

		bool sendWeatherData(uint8_t destinationAddress, WeatherData &data, LEDutilities *led = nullptr);
		bool sendData(uint8_t destinationAddress, uint8_t * data, uint8_t length, LEDutilities *led = nullptr);

		inline void sleep(void) const { driver->sleep(); }

		void printRoutingTable(void);
		int16_t getLastRssi(void);

	private:
		RH_RF22 *driver;
		RHMesh *manager;

		uint8_t buff[20];
		const uint8_t buffLen = 20;
		uint8_t from = 0xFF;

		#if WATCHDOG_ENABLED
			uint8_t tx_errors_counter = 0;
		#endif

};

#endif
