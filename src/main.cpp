#include <Arduino.h>
#include "painlessMesh.h"
#include "EEPROM.h"
#include "uwb.h"
#include "config.h"
#include "box_control.h"
#include "mesh_network.h"



void setup() {
  Serial.begin(115200);
  
  pinMode(wakeupPin, INPUT_PULLUP); 

  Serial.println("\nInitialize EEPROM Library\n");
  EEPROM.begin(EEPROM_SIZE);
  
  esp_sleep_enable_ext0_wakeup((gpio_num_t)wakeupPin, LOW);

  initBoxControl(); 
  init_UWB();
  setUpMesh();
}

void loop(){
  uwb_main();
  mesh.update();
}