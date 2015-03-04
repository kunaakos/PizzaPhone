#include <Arduino.h>

#include "hook.h"

#define PIN_HOOK 2

HOOK::HOOK()
{
    // nobody understands how constructors work in Arduino
}

void HOOK::init()
{
    pinMode(PIN_HOOK, INPUT);
    digitalWrite(PIN_HOOK, HIGH);
}

void HOOK::check()
{
    currentState = digitalRead(PIN_HOOK);

    // check for state change
    if (currentState != lastState) {
        lastDebounceTime = millis();
    }

    // check if enough time has passed since last state change
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (currentState != debouncedState) {
            debouncedState = currentState; // state change
            stateToReturn = currentState + 2;
        }
    }

    lastState = currentState;
}

uint8_t HOOK::state()
{
    uint8_t temp = stateToReturn;
    if (stateToReturn > 1)
    {
        stateToReturn -= 2; // JUST_HUNG_UP and JUST_PICKED_UP only reported once
    }
    return temp;
}