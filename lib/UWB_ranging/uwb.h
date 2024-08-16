#ifndef UWB_H
#define UWB_H
#include "DW1000Ranging.h"
#include "DW1000.h"

//------------------UWB------------------------
// #define SPI_SCK             18
// #define SPI_MISO            19
// #define SPI_MOSI            23
// #define DW_CS               4
// #define PIN_RST             27
// #define PIN_IRQ             34
// #define PIN_SS              4

#define SPI_SCK             12
#define SPI_MISO            13
#define SPI_MOSI            11
#define DW_CS               10
#define PIN_RST             8
#define PIN_IRQ             6
#define PIN_SS              10

// TAG antenna delay defaults to 16384
// leftmost two bytes below will become the "short address"
#define ANCHOR_ADD "01:00:5B:D5:A9:9A:E2:9C"

// prototypes
void newRange();
void newDevice(DW1000Device *device);
void inactiveDevice(DW1000Device *device);
void init_UWB();
void uwb_main();

#endif