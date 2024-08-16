#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "painlessMesh.h"
#include "EEPROM.h"
#include "uwb.h"

// Scheduler for controlling personal tasks
extern Scheduler userScheduler;
extern painlessMesh mesh;

// Mesh network configuration
#define MESH_PREFIX     "HANGAR1" 
#define MESH_PASSWORD   "project2024"
#define MESH_PORT       5551   

// Global variables
extern bool boxState; // Caja cerrada
extern uint8_t wakeupPin;
extern bool release_node;
extern bool oneTime;
extern uint8_t pulses_count;

// EEPROM configuration
#define EEPROM_SIZE 96
extern const int pulses_address;
extern const int status_address;

// LOTO box configuration
#define LOTO          35
extern const int freq;
extern const int ledChannel;
extern const int resolution;

// Other variables
extern RTC_DATA_ATTR bool start;

#endif
