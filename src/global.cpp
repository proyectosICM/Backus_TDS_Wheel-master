#include "global.h"

extern Scheduler userScheduler;
extern painlessMesh mesh;

// Estado de la caja LOTO
bool boxState = false; 
uint8_t wakeupPin = 2;
bool release_node = false;
bool oneTime = true;
uint8_t pulses_count = 0;

// EEPROM configuration
const int pulses_address = 0;
const int status_address = 4;

// Configuración de PWM
const int freq = 400;
const int ledChannel = 0;
const int resolution = 8;

RTC_DATA_ATTR bool start = true;