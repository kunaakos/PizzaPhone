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

time_t alarmTime;

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
        Serial.println("Picked up.");
        #endif

        bell.state(false); // bell must stop, no matter what
        speak.currentTime(hour(), minute());

        alarmTime = clock.alarmTime();

        switch (clock.alarmState()) {
            case ALARM_OFF:
                speak.alarmSettings();
                break;
            case ALARM_ACTIVE:
                speak.alarmSettings(hour(alarmTime), minute(alarmTime));
                break;
            case ALARM_SNOOZED:
                speak.alarmSettings(hour(alarmTime), minute(alarmTime));
                break;
            case ALARM_TRIGGERED:
                // snooze/deactivate routine
                clock.snoozeAlarm(2);
                break;
        }
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