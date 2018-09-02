
#ifndef _MY_SETTINGS_h
#define _MY_SETTINGS_h

// Watchdog
////////
#define WATCHDOG_ENABLED true

// Pins
#define SLAVE_SELECT_PIN	3
#define INTERRUPT_PIN		2

#define DELAY_BETWEEN_DHT22_READINGS_MS	2020
#define MAX_DHT22_TRIED_READINGS		11

// 20 min = 20 min * 60 s * 1000 ms = 1'200'000 ms
//  5 min =  5 min * 60 s * 1000 ms =   300'000 ms
//  4 min =  4 min * 60 s * 1000 ms =   240'000 ms
//  3 min =  3 min * 60 s * 1000 ms =   180'000 ms
#define SENDING_PERIOD					180000
#define LED_BLINKING_PERIOD_RECEIVER	44
#define LED_BLINKING_PERIOD				8000

#define DEBUG_ENABLED		false
#define SINK_ENABLE_SENSOR	false

#endif
