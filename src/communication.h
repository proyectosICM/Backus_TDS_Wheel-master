#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include "painlessMesh.h"
#include <DW1000.h>

extern const String MESSAGE_CHANGE;
extern const String MESSAGE_RELEASED;
extern const int MAX_PULSE_COUNT;

extern volatile boolean received;

void setupCommunication();
bool sendMessage(String msg);
void transmitir(String message);
void receiver();


void receivedCallback();

void handleReceived();



#endif // COMMUNICATION_H
