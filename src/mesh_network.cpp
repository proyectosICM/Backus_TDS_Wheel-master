#include "mesh_network.h"
#include "box_control.h"
#include "EEPROM.h"

const String MESSAGE_CHANGE = "Change";
const String MESSAGE_RELEASED = "Released";
const int MAX_PULSE_COUNT = 3;

void newConnectionCallback(uint32_t nodeId)
{
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
  if (oneTime)
  {
    // Funcion de alejamiento ()
    // Si se aleja llamar a handleChangeMessage para sumar el pulso y abrir
    // pulses_count++;
    EEPROM.writeInt(pulses_address, pulses_count);
    EEPROM.commit();
    Serial.println("One Time activated");
    oneTime = false;
  }
}

void handleChangeMessage()
{
  pulses_count++;

  switch (pulses_count)
  {
  case 1:
    Serial.println("Abriendo Caja LOTO");
    changeBoxState(true);
    break;
  case 2:
    Serial.println("Cerrando Caja LOTO");
    changeBoxState(false);
    break;
  case 3:
    Serial.println("Reabriendo Caja LOTO");
    changeBoxState(true);
    break;
  default:
    Serial.println("Default");
    break;
  }

  EEPROM.writeInt(pulses_address, pulses_count);
  EEPROM.commit();
}

void handleReleasedMessage()
{
  if (pulses_count >= MAX_PULSE_COUNT)
  {
    Serial.println("Cerrando Caja LOTO");
    changeBoxState(false);
    EEPROM.writeInt(status_address, 0);
    EEPROM.writeInt(pulses_address, 0);
    EEPROM.commit();
    release_node = true;
    esp_deep_sleep_start();
  }
}

void receivedCallback(uint32_t from, String &msg)
{
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());

  if (msg == MESSAGE_CHANGE)
  {
    handleChangeMessage();
  }
  else if (msg == MESSAGE_RELEASED)
  {
    handleReleasedMessage();
  }
}

/*- CODIGO ANTERIOR -*/
/*- PENDIENTE A ELIMINACION -*/
/*-  -*/
void receivedCallback2(uint32_t from, String &msg)
{
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  if (strcmp("Change", msg.c_str()) == 0)
  {
    Serial.println("Cambio de estado LOTO BOX");
    pulses_count++;
    changeBoxState(true);
    EEPROM.writeInt(pulses_address, pulses_count);
    EEPROM.commit();
  }
  else if (strcmp("Released", msg.c_str()) == 0 && pulses_count >= 3)
  {
    if (boxState)
    {
      Serial.println("Cerrando Caja LOTO");
      changeBoxState(false);
      boxState = false;
    }
    EEPROM.writeInt(status_address, 0);
    EEPROM.writeInt(pulses_address, 0);
    EEPROM.commit();
    release_node = true;
    esp_deep_sleep_start();
  }
}

void setUpMesh()
{
  Serial.println("Starting WiFi Mesh");
  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onReceive(&receivedCallback);
}

bool sendMessage(String msg)
{
  return mesh.sendBroadcast(msg);
}