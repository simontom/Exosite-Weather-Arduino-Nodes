
#ifndef MY_PROJECT_ADDRESSES_h
#define MY_PROJECT_ADDRESSES_h

#include <RH_RF22.h>
#include <avr/wdt.h>

// Addresses
/////////////////////////////////////////////////////////

#define SINK_NODE_ADDR		1	/* Main node connected to RasPi */

#define OUTSIDE_NODE_ADDR	10	/* Measures temperature and humidity */
#define BATHROOM_NODE_ADDR	11	/* My bathroom */
#define BEDROOM_NODE_ADDR	12	/* My bedroom */
#define LIVROOM_NODE_ADDR	13	/* Sister's living room */
#define HALLWAY_NODE_ADDR	14	/* Measures pressure and temperature */
#define LIVROOM2_NODE_ADDR	15	/* Mom's living room */
#define LIGHT_NODE_ADDR		16	/* Measures UV Index and LUXes outside */
#define BEDROOM2_NODE_ADDR	17	/* Kids' bedroom */

#define RGB_NODE_ADDR		111	/* Controlled RGB node */
#define TESTING_NODE_ADDR	222	/* Used for testing -- if any node is down ? */

// Settings
/////////////////////////////////////////////////////////

#define TIMEOUT	320
#define RETRIES	3

#define SYNC_WORD_1	0xD3
#define SYNC_WORD_2	0x3D
const uint8_t SYNC_WORDS[] = { SYNC_WORD_1, SYNC_WORD_2 };

#define RADIO_MODEM_CONFIG	RH_RF22::GFSK_Rb19_2Fd9_6
#define RADIO_POWER			RH_RF22_TXPOW_8DBM
#define RADIO_POWER_LOWER	RH_RF22_TXPOW_5DBM

#define MAX_TX_ERRORS_BEFORE_RESET		4

#define DELAY_BETWEEN_DHT22_READINGS_MS	2020
#define MAX_DHT22_TRIED_READINGS		11

// 20 min = 20 min * 60 s * 1000 ms = 1'200'000 ms
//  5 min =  5 min * 60 s * 1000 ms =   300'000 ms
//  4 min =  4 min * 60 s * 1000 ms =   240'000 ms
//  3 min =  3 min * 60 s * 1000 ms =   180'000 ms
#define SENDING_PERIOD					180000
#define DEBUG_BLINKING_PERIOD_RECEIVER	44
#define DEBUG_BLINKING_PERIOD			8000

#define DHT_MIN_TIME_WINDOW_MS		2500
unsigned long dhtLastRead = 0;

#define DEBUG_ENABLED		false
#define USE_MESH_LIBRARY	true
#define SINK_ENABLE_SENSOR	false

#define WATCHDOG_RESET_TIME	WDTO_8S

// Data Types
/////////////////////////////////////////////////////////

typedef enum {
	wdt_16ms = 0,
	wdt_32ms,
	wdt_64ms,
	wdt_128ms,
	wdt_250ms,
	wdt_500ms,
	wdt_1s, wdt_2s,
	wdt_4s, wdt_8s
} wdt_prescalar_e;

typedef struct {
	float temperature;
	float humidity;
	float pressure;
	uint16_t volt_mv;
} WeatherStruct;

// Common Variables
/////////////////////////////////////////////////////////

WeatherStruct weatherData;
uint8_t* weatherDataPtr = (uint8_t*)&weatherData;
const uint8_t weatherDataLen = sizeof(WeatherStruct);

uint8_t buff[18];
const uint8_t buffLen = 18;
uint8_t from = 0xFF;

// Helper Defines And Functions
/////////////////////////////////////////////////////////

#define STOP_HERE()				while(true) { ; }
#define STOP_HERE_LED(_LED_)	while(true) { _LED_.blinkLed(1, 444, 1555); }

#define TIME_DRIVEN_EVENT(now_, last_, period_, body_)	\
	do { \
		if ((now_ - last_) >= (period_)) { \
			last_ = now_; \
			body_ \
		} \
	} while (false)

#define PROTECT_WITH_WDT(_wdt_enabled_, _body_)	\
	do { \
		if (_wdt_enabled_) { \
			wdt_enable(WATCHDOG_RESET_TIME); \
			wdt_reset(); \
		} \
		_body_ \
		if (_wdt_enabled_) { \
			wdt_disable(); \
		} \
	} while(false)

#define DEBUG_WITH_LED(_led_, _body_)	\
	do { \
		_led_.onLed(); \
		_body_; \
		_led_.offLed(); \
	} while(false)


void rebootArduino() {
	//wdt_enable(WATCHDOG_RESET_TIME);
	wdt_enable(WDTO_2S);
	for ( ; ; ) { ; }
}


int freeRam(void) {
	extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}
void printFreeRam(void) {
	Serial.print(F("SRAM: "));
	Serial.println(freeRam());
	delay(11);
}

#endif // MY_PROJECT_ADDRESSES_h
