#include "messageHandlers.h"
#include "box_control.h"
#include "EEPROM.h"
#include <SPI.h>
#include "ranging.h"
#include "global.h"
#include "communication.h"


// Mensajes que se reciben
const String MESSAGE_CHANGE = "Change";
const String MESSAGE_RELEASED = "Released";
const String MESSAGE_CONNECT = "Connect";

// Mensajes que se envían
const String MESSAGE_ISCONNECT = "IsConnect";

const int MAX_PULSE_COUNT = 3;
 
// Manejador de mensaje no reconocido
void handleUnknownMessage() {
    Serial.println("Mensaje no reconocido");
}

// Manejador de nueva conexión
void handleConnectMessage() {
    //Enviar mensaje de conexion
    transmitir(MESSAGE_ISCONNECT);
    //Enviar la cantidad de pulsos almacenados
    transmitir(String(pulses_count));

    Serial.println("Mensaje enviado: IsConnect");
}

// Manejador de pulsos
void handleChangeMessage() {
    pulses_count++;

    switch (pulses_count) {
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

// Manejador de liberación
void handleReleasedMessage() {
    if (pulses_count >= MAX_PULSE_COUNT) {
        Serial.println("Cerrando Caja LOTO");
        changeBoxState(false);
        EEPROM.writeInt(status_address, 0);
        EEPROM.writeInt(pulses_address, 0);
        EEPROM.commit();
        release_node = true;
        esp_deep_sleep_start();
    }
}

// Mapa de mensajes a manejadores
const MessageHandlerMap messageHandlers[] = {
    {MESSAGE_CHANGE, handleChangeMessage},
    {MESSAGE_RELEASED, handleReleasedMessage},
    {MESSAGE_CONNECT, handleConnectMessage}
};

// Especificación del tamaño del array
const size_t messageHandlersSize = sizeof(messageHandlers) / sizeof(messageHandlers[0]);
