#include "box_control.h"
#include "EEPROM.h"
#include "global.h"

// Función para inicializar el control de la caja LOTO
void initBoxControl()
{
    // Configura el canal de PWM
    ledcSetup(ledChannel, freq, resolution);
    ledcAttachPin(LOTO, ledChannel);
    // Establece el ángulo inicial del servomotor
    ledcWrite(ledChannel, 255 * 52 / 100);

    if (EEPROM.readInt(status_address) == 0)
    {
        Serial.println("Entrando en modo Deep Sleep");
        esp_deep_sleep_start();
    }

    // Lee el número de pulsos almacenados en la EEPROM
    pulses_count = EEPROM.readInt(pulses_address);
    Serial.print("Pulsos contados: ");
    Serial.println(pulses_count);
    EEPROM.writeInt(status_address, 1);
    EEPROM.commit();
    Serial.print("Ahora en el estado: ");
    Serial.println(EEPROM.readInt(status_address));
}

const int pwmValues[2] = {
    255 * 52 / 100,  // Valor para la caja cerrada
    255 * 18 / 100   // Valor para la caja abierta
};

void changeBoxState(bool open)
{
    // Selecciona el valor PWM basado en el estado deseado
    int pwmValue = open ? pwmValues[1] : pwmValues[0];
    
    Serial.println(open ? "LOTO box opened" : "LOTO box closed");
    ledcWrite(ledChannel, pwmValue);
    boxState = open;
}
