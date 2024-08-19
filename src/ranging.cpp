#include "ranging.h"

char anchor_addr[] = "84:00:5B:D5:A9:9A:E2:9C"; // Dirección MAC del anchor
uint16_t Adelay = 16580; // Retraso de la antena calibrado para el anchor

void setupSPI() {
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
}

void setupDW1000() {
    DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ);
    DW1000.setAntennaDelay(Adelay);
    DW1000Ranging.attachNewRange(newRange);
    DW1000Ranging.attachNewDevice(newDevice);
    DW1000Ranging.attachInactiveDevice(inactiveDevice);
    DW1000Ranging.startAsAnchor(anchor_addr, DW1000.MODE_LONGDATA_RANGE_LOWPOWER, false);
}

void newRange() {
    Serial.print(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
    Serial.print(", ");
    Serial.println(DW1000Ranging.getDistantDevice()->getRange());
}

void newDevice(DW1000Device *device) {
    Serial.print("Device added: ");
    Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device *device) {
    Serial.print("Delete inactive device: ");
    Serial.println(device->getShortAddress(), HEX);
}
