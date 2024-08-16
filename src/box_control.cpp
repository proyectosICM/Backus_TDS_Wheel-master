#include "box_control.h"
#include "EEPROM.h"

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

void changeBoxState()
{
    if (!boxState)
    {
        Serial.println("LOTO box openned");
        ledcWrite(ledChannel, 255 * 18 / 100);
    }
    else
    {
        Serial.println("LOTO box closed");
        ledcWrite(ledChannel, 255 * 52 / 100);
    }
    boxState = !boxState;
}
