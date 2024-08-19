#include <Arduino.h>
#include "painlessMesh.h"
#include "EEPROM.h"
#include "config.h"
#include "box_control.h"
#include "ranging.h"
#include "communication.h"
#include "global.h"

void setup()
{
  Serial.begin(115200);

  pinMode(wakeupPin, INPUT_PULLUP);

  Serial.println("\nInitialize EEPROM Library\n");
  EEPROM.begin(EEPROM_SIZE);

  esp_sleep_enable_ext0_wakeup((gpio_num_t)wakeupPin, LOW);

  // Inicializar SPI y DW1000
  setupSPI();
  setupDW1000();

  // Inicializar el control de la caja
  initBoxControl();
}

void loop()
{
  // mesh.update();

  DW1000Ranging.loop();
  receivedCallback();
}