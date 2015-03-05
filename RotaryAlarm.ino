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
#include "speak.h"

#include "debug.h"

SOUND sound;
DIAL dial;
HOOK hook;
CLOCK clock;
BELL bell;
SPEAK speak;

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
    speak.init();

    #ifdef DEBUG
    Serial.println("Initialized.");
    bell.status(true);
    #endif
}

void loop()
{
    sound.check();
    dial.check();
    hook.check();
    clock.check();
    bell.check();
    speak.check();

    uint8_t hookState = hook.state();
    
    // picked up hook
    if (hookState == 3)
    {
        #ifdef DEBUG
        bell.status(false);
        Serial.println("Picked up.");
        #endif

        speak.currentTime(hour(), minute());
    }

    // dialed a number while receiver is picked up
    if (dial.available() && (hookState == 3 || hookState == 1))
    {
        #ifdef DEBUG
        Serial.print("dialed: ");
        Serial.println(dial.lastNumber());
        #endif

        speak.stopSpeaking();
        sound.stopPlaying();
    }

    // hung up
    if (hookState == 2)
    {
        #ifdef DEBUG
        Serial.println("Hung up.");
        bell.status(true);
        #endif

        speak.stopSpeaking();
        sound.stopPlaying();
    }

    // play stuff if there is stuff to play
    if (speak.isSpeaking() && !sound.isPlaying())
    {
        sound.startPlaying(speak.soundBiteFolderNr(), speak.soundBiteFileNr());
        speak.nextSoundBite();
    }
}