#ifndef RANGING_H
#define RANGING_H

#include <SPI.h>
#include "DW1000Ranging.h"
#include "DW1000.h"


// Definición de pines para rangin.h
#define SPI_SCK    12
#define SPI_MISO   13
#define SPI_MOSI   11
#define DW_CS      10
#define PIN_RST    8
#define PIN_IRQ    6
#define PIN_SS     10

// Dirección MAC del anchor
extern char anchor_addr[];

// Retraso de la antena calibrado para el anchor
extern uint16_t Adelay;

void setupSPI();
void setupDW1000();
void newRange();
void newDevice(DW1000Device *device);
void inactiveDevice(DW1000Device *device);

#endif
