#ifndef GLOBAL_H
#define GLOBAL_H

#include <Arduino.h>
#include "painlessMesh.h"
#include "EEPROM.h"

// Scheduler for controlling personal tasks
extern Scheduler userScheduler;
extern painlessMesh mesh;

// Variables para envio y recepcion de mensajes
extern volatile boolean received;
extern String message;

// Estado de la caja LOTO
extern bool boxState;          
extern uint8_t wakeupPin;
extern bool release_node;
extern bool oneTime;
extern uint8_t pulses_count;

// EEPROM configuration

extern const int pulses_address;
extern const int status_address;

// Configuración de PWM
extern const int freq;
extern const int ledChannel;
extern const int resolution;

// Other variables
extern RTC_DATA_ATTR bool start;
#endif 
