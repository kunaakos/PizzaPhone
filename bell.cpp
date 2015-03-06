#include <Arduino.h>

#include <PhoneBell.h> // https://github.com/kunaakos/PhoneBell

#include "bell.h"

#define BELL_PIN_1 3
#define BELL_PIN_2 4
#define FREQ 8

PhoneBell phoneBell = PhoneBell(BELL_PIN_1, BELL_PIN_2, FREQ);

BELL::BELL()
{
    // nobody understands how constructors work in Arduino
}

void BELL::init()
{
    phoneBell.intermittentRinging(true);
}

void BELL::check()
{
    phoneBell.update();
}

void BELL::state(bool ringing)
{
    isRinging = ringing;
    if (isRinging)
    {
        phoneBell.startRinging();
    }
    else
    {
        phoneBell.stopRinging();
    }
}

bool BELL::state()
{
    return isRinging;
}