#include <Arduino.h>

#include <SoftwareSerial.h>
#include <RotaryDialer.h> // https://github.com/kunaakos/RotaryDialMM
#include <DS3232RTC.h> // https://github.com/JChristensen/DS3232RTC
#include <Time.h>     
#include <Wire.h>
#include <PhoneBell.h> // https://github.com/kunaakos/PhoneBell

#include "sound.h"
#include "dial.h"
#include "hook.h"
#include "clock.h"
#include "bell.h"

#define DEBUG

SOUND sound;
DIAL dial;
HOOK hook;
CLOCK clock;
BELL bell;

void setup()
{
    #ifdef DEBUG
    Serial.begin(9600);
    Serial.println("Begin init.");
    #endif

    sound.init();
    dial.init();
    hook.init();
    clock.init();
    bell.init();

    #ifdef DEBUG
    Serial.println("Initialized.");
    time_t alarmTime = clock.getAlarm();
    Serial.println(hour(alarmTime));
    Serial.println(minute(alarmTime));
    bell.status(true);
    #endif
}

void loop()
{
    sound.check();
    dial.check();
    hook.check();
    bell.check();

    uint8_t hookState = hook.state();
    
    if (!sound.isPlaying() && hookState == 3)
    {
        #ifdef DEBUG
        Serial.println("Picked up.");
        bell.status(false);
        #endif

        sound.startPlaying(20, 8);
    }

    if (dial.available())
    {
        #ifdef DEBUG
        Serial.print("dialed: ");
        Serial.println(dial.lastNumber());
        #endif

        sound.stopPlaying();
    }

    if (hookState == 2)
    {
        #ifdef DEBUG
        Serial.println("Hung up.");
        bell.status(true);
        #endif

        sound.stopPlaying();
    }
}