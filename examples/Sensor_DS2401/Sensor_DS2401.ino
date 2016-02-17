/*
 *    Example-Code that emulates a DS2401 used as a binary button (like reed-contact - power on, power off)
 *    Tested with https://github.com/PaulStoffregen/OneWire on the other side as Master
 */

#include "OneWireHub.h"
#include "DS2401.h"  // Serial Number

const uint8_t led_PIN       = 13;         // the number of the LED pin
const uint8_t OneWire_PIN   = 8;

OneWireHub  hub = OneWireHub(OneWire_PIN);
auto ds2401A = DS2401( 0x01, 0x0D, 0x02, 0x04, 0x00, 0x01, 0x0A );    // Work - Serial Number
auto ds2401B = DS2401( 0x01, 0x0D, 0x02, 0x04, 0x00, 0x01, 0x0B );    // Work - Serial Number
auto ds2401C = DS2401( 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C );

bool blinking()
{
    const  uint32_t interval    = 50000;        // interval at which to blink (milliseconds)
    static uint32_t nextMillis  = millis();     // will store next time LED will updated

    uint32_t currentMillis = millis();

    if (currentMillis > nextMillis)
    {
        static uint8_t ledState = LOW;          // ledState used to set the LED
        nextMillis = currentMillis + interval;  // save the last time you blinked the LED
        if (ledState == LOW)    ledState = HIGH;
        else                    ledState = LOW;
        digitalWrite(led_PIN, ledState);
        return 1;
    }
    return 0;
}


void setup()
{
    Serial.begin(115200);
    Serial.println("OneWire-Hub DS2401 Serial Number used as iButton");
    hub.attach(ds2401C); // always there
}

void loop()
{
    hub.waitForRequest();

    // Blink triggers the state-change
    if (blinking())
    {
        static bool flipflop = 0;

        if (flipflop)
        {
            flipflop = 0;
            hub.detach(ds2401A);
            hub.attach(ds2401B);
            Serial.println("B is active");
        }
        else
        {
            flipflop = 1;
            hub.detach(ds2401B);
            hub.attach(ds2401A);
            Serial.println("A is active");
        }
    }
}