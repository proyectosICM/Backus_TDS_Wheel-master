#include "config.h"

// Initialize global variables
Scheduler userScheduler;
painlessMesh mesh;

bool boxState = false; // Caja cerrada
uint8_t wakeupPin = 2;

bool release_node = false;
bool oneTime = true;
uint8_t pulses_count = 0;

const int pulses_address = 0;
const int status_address = 4;

RTC_DATA_ATTR bool start = true;

const int freq = 400;
const int ledChannel = 0;
const int resolution = 8;
