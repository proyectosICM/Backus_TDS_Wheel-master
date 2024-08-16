#include "mesh_network.h"
#include "box_control.h"
#include "EEPROM.h"

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
    if(oneTime){
        changeBoxState();
        pulses_count++;
        EEPROM.writeInt(pulses_address, pulses_count);
        EEPROM.commit();
        Serial.println("One Time activated");
        oneTime = false;
    }
}

void receivedCallback(uint32_t from, String &msg) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  if(strcmp("Change", msg.c_str()) == 0) {
      Serial.println("Cambio de estado LOTO BOX");
      pulses_count++;
      changeBoxState();
      EEPROM.writeInt(pulses_address, pulses_count);
      EEPROM.commit();
  } else if (strcmp("Released", msg.c_str()) == 0 && pulses_count >= 3) {
      if(boxState) {
          Serial.println("Cerrando Caja LOTO");
          changeBoxState();
          boxState = false;
      }
      EEPROM.writeInt(status_address, 0);
      EEPROM.writeInt(pulses_address, 0);
      EEPROM.commit();
      release_node = true;
      esp_deep_sleep_start();
  }
}

void setUpMesh() {
  Serial.println("Starting WiFi Mesh");
  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onReceive(&receivedCallback);
}

bool sendMessage(String msg) {
  return mesh.sendBroadcast(msg);
}
