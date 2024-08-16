#include <Arduino.h>
#include "painlessMesh.h"
#include "EEPROM.h"
#include "uwb.h"

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

#define   MESH_PREFIX     "HANGAR1" // Hangar#, # is the Hangar's bay number
#define   MESH_PASSWORD   "project2024"
#define   MESH_PORT       5551      // 555#, # is the Hangar's bay number

bool boxState = false; // Caja cerrada
//uint8_t wakeupPin =  12;
uint8_t wakeupPin =  2;

bool release_node = false;
bool oneTime = true;
uint8_t pulses_count = 0;

// Direccion en la memoria EEPROM
#define EEPROM_SIZE 96
const int pulses_address = 0;
const int status_address = 4;
RTC_DATA_ATTR bool start = true;

// Direccion de la caja LOTO
//#define LOTO          15
#define LOTO          35
const int freq =      400;
const int ledChannel = 0;
const int resolution = 8; //Resolution 8, 10, 12, 15

//--------------Prototypes-----------------
void newConnectionCallback(uint32_t nodeId);
void changedConnectionCallback(); 
bool sendMessage(String msg);

void ChangeBoxState(){
  if(!boxState){
    Serial.println("LOTO box openned"); 
    // Abrir Caja LOTO
    ledcWrite(ledChannel, 255*18/100); // -> 90°
  }else{
    Serial.println("LOTO box closed"); 
    //Cerrar Caja LOTO
    ledcWrite(ledChannel, 255*52/100); // -> 0°
  }
  boxState = !boxState;
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
    if(oneTime){ // && nodeId != tablero_id
      ChangeBoxState(); // agregar el distanciamiento
      pulses_count++;
      EEPROM.writeInt(pulses_address, pulses_count);
      EEPROM.commit();
      Serial.println("One Time activated");
      oneTime = false;
    }
}

/*
void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
  if(release_node){
    release_node = false;
    Serial.println("Entrando en modo reposo");
    //Entra en SleepMode;
    esp_deep_sleep_start(); // Es posible que lo llevemos arriba // actualizacion: no funciona
  }
}
*/

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  if(strcmp("Change", msg.c_str())==0){
    Serial.println("Cambio de estado LOTO BOX");
    pulses_count++;
    ChangeBoxState();
    EEPROM.writeInt(pulses_address, pulses_count);
    EEPROM.commit();
  }else if (strcmp("Released", msg.c_str())==0 && pulses_count>=3){
    /*
    if(!sendMessage("ACK")){
      Serial.println("ERROR...");
    }
    */
    if(boxState){ // Si la caja LOTO se encuentra abierta, se cierra antes de entrar en DeepSleep
      Serial.println("Cerrando Caja LOTO");
      ChangeBoxState();
      boxState = false;
    }
    EEPROM.writeInt(status_address, 0);
    EEPROM.writeInt(pulses_address, 0);
    EEPROM.commit();
    Serial.print("Pulsos reiniciados a: ");
    Serial.println(EEPROM.readInt(pulses_address));
    Serial.println("Se liberó el monta carga");
    release_node = true; 
    //start = true;
    esp_deep_sleep_start();
  }
}

bool sendMessage(String msg) {
  return mesh.sendBroadcast( msg );
}

// NOta: En la segunda versión del MESH se comentó el changedConnection() 
// y se agregó esp_deep_sleep_start() dentro del receivedCallback().
void setUpMesh(){
  Serial.println("Starting WiFi Mesh");
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init(MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  //mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onReceive(&receivedCallback);
  // This node and all other nodes should ideally know the mesh contains a root, so call this on all nodes
  //mesh.setContainsRoot(true);
}

void setup() {
  Serial.begin(115200);
  // Deep Sleep Mode configuration
  pinMode(wakeupPin, INPUT_PULLUP); // When using a normally open push button
  Serial.println("\nInitialize EEPROM Library\n");
  EEPROM.begin(EEPROM_SIZE);
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(LOTO, ledChannel);
  ledcWrite(ledChannel, 255*52/100);
  esp_sleep_enable_ext0_wakeup((gpio_num_t) wakeupPin, LOW);

  Serial.print("Estado en: ");
  Serial.println(EEPROM.readInt(status_address));
  
  if(EEPROM.readInt(status_address)==0){
    if(start){
      start = false;
      Serial.println("Entrando en modo Deep Sleep");
      esp_deep_sleep_start();
    }
  }

  pulses_count = EEPROM.readInt(pulses_address);
  Serial.print("Pulsos contados: ");
  Serial.println(pulses_count);
  EEPROM.writeInt(status_address, 1);
  EEPROM.commit();
  Serial.print("Ahora en el estado: ");
  Serial.println(EEPROM.readInt(status_address));

  init_UWB();
  setUpMesh();

}

void loop(){
  uwb_main();
  mesh.update();
}