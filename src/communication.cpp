#include "communication.h"
#include "box_control.h"
#include "EEPROM.h"
#include <SPI.h>
#include "ranging.h"
#include "global.h"
#include "messageHandlers.h"

volatile boolean received = false;
volatile boolean sent = false;
String message;

void receivedCallback()
{
    if (received)
    {
        DW1000.getData(message);
        Serial.println("Mensaje recibido: " + message);

        // Buscar el manejador de mensajes
        bool handled = false;
        for (size_t i = 0; i < messageHandlersSize; i++)
        {
            if (message == messageHandlers[i].message)
            {
                messageHandlers[i].handler();
                handled = true;
                break;
            }
        }

        if (!handled)
        {
            handleUnknownMessage();
        }

        received = false;
    }
}

void setupCommunication()
{
    DW1000.begin(PIN_IRQ, PIN_RST);
    DW1000.select(PIN_SS);
    DW1000.newConfiguration();
    DW1000.setDefaults();
    DW1000.setDeviceAddress(6);
    DW1000.setNetworkId(10);
    DW1000.enableMode(DW1000.MODE_LONGDATA_RANGE_LOWPOWER);
    DW1000.commitConfiguration();

    DW1000.attachReceivedHandler(handleReceived);
    receiver();
}

bool sendMessage(String msg)
{
    return mesh.sendBroadcast(msg);
}

void transmitir(String message)
{
    DW1000.newTransmit();
    DW1000.setDefaults();
    DW1000.setData(message);
    DW1000.startTransmit();
}

void receiver()
{
    DW1000.newReceive();
    DW1000.setDefaults();
    DW1000.receivePermanently(true);
    DW1000.startReceive();
}

void handleReceived()
{
    received = true;
}

void handleSent()
{
    sent = true;
}