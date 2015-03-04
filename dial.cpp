#include <Arduino.h>
#include <RotaryDialer.h> // https://github.com/kunaakos/RotaryDialMM

#include "dial.h"

// rotary dial config
#define PIN_READY 9
#define PIN_PULSE 10
RotaryDialer dialer = RotaryDialer(PIN_READY, PIN_PULSE);

DIAL::DIAL()
{
    // nobody understands how constructors work in Arduino
}

void DIAL::init()
{
    // nothing to init yet
}

void DIAL::check()
{
    if (dialer.update())
    {
        numberAvailable = true;
    }
}

boolean DIAL::available()
{
    return numberAvailable;
}

byte DIAL::lastNumber()
{
    numberAvailable = false;
    return dialer.getLastNumber();
}