#ifndef MESSAGE_HANDLERS_H
#define MESSAGE_HANDLERS_H

#include <Arduino.h>

// Definición del tipo de puntero a función
typedef void (*MessageHandler)();

// Estructura para mapear mensajes a manejadores
struct MessageHandlerMap {
    const String message;
    MessageHandler handler; 
};

// Mensajes que se reciben
extern const String MESSAGE_CHANGE;
extern const String MESSAGE_RELEASED;
extern const String MESSAGE_CONNECT;

// Mensajes que se envían
extern const String MESSAGE_ISCONNECT;

// Declaración de funciones handler
void handleConnectMessage();
void handleChangeMessage();
void handleReleasedMessage();
void handleUnknownMessage();

// Declaración del array y su tamaño
extern const MessageHandlerMap messageHandlers[];
extern const size_t messageHandlersSize;

#endif
