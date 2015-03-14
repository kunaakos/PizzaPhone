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
#include "inputHelper.h"

#include "debug.h"

SOUND sound;
DIAL dial;
HOOK hook;
CLOCK clock;
BELL bell;
SPEAK speak;
INPUTHELPER inputHelper;

void setup()
{
    #ifdef DEBUG
    Serial.begin(9600);
    Serial.println("=============================================");
    Serial.println("SETUP - Begin init.");
    #endif

    randomSeed(analogRead(A0));

    sound.init();
    dial.init();
    hook.init();
    clock.init();
    bell.init();
    speak.init();
    inputHelper.init();

    #ifdef DEBUG
    Serial.println("SETUP - Initialized.");
    Serial.println("=============================================");

    // setting test alarm!
    clock.setAlarm(hour(), minute()+1);
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
    inputHelper.check();

    switch(hook.state()) {
        case JUST_PICKED_UP:
            justPickedUp();
            break;
        case PICKED_UP:
            pickedUp();
            break;
        case JUST_HUNG_UP:
            justHungUp();
            break;
        case HUNG_UP:
            hungUp();
            break;
    }
    
    playStuff();
}

void justPickedUp()
{
    #ifdef DEBUG
    Serial.println("MAIN - Picked up.");
    #endif

    bell.state(NOT_RINGING);

    time_t alarmTime = clock.alarmTime();

    switch (clock.alarmState()) {
        case ALARM_OFF:
            speak.currentTime(hour(), minute());
            speak.alarmOff();
            inputHelper.startReadingTime();
            break;
        case ALARM_ACTIVE:
            speak.currentTime(hour(), minute());
            speak.alarmSetTo(hour(alarmTime), minute(alarmTime));
            inputHelper.startReadingTime();
            break;
        case ALARM_SNOOZED:
            speak.alarmSnoozed(inputHelper.startReadingCode());
            break;
        case ALARM_TRIGGERED:
            speak.promptForDeactivationCode(inputHelper.startReadingCode());
            break;
    }
}

void pickedUp()
{
    time_t alarmTime;

    if (dial.available())
    {
        speak.stopSpeaking();
        sound.stopPlaying();

        if (inputHelper.isReading())
            inputHelper.addDigit(dial.lastNumber());
        else
            dial.lastNumber(); // dump number
    }

    if (inputHelper.hasResult())
    {
        switch (inputHelper.status()) {
            case CODE_OK:
                #ifdef DEBUG
                Serial.println("MAIN - Alarm deactivated.");
                #endif

                clock.unSetAlarm();
                speak.message(CODE_OK);
                inputHelper.stopReading();
                break;

            case CODE_INCORRECT:
                #ifdef DEBUG
                Serial.println("MAIN - Incorrect deactivation code.");
                #endif

                speak.message(CODE_INCORRECT);
                speak.promptForDeactivationCode(inputHelper.startReadingCode());
                break;

            case TIME_OK:
                alarmTime = inputHelper.getTime();
                clock.setAlarm(hour(alarmTime), minute(alarmTime));
                speak.alarmSetTo(hour(alarmTime), minute(alarmTime));
                inputHelper.stopReading();
                break;

            case NUMBER_OK:
                // l8rz
                break;
        }
    }
}

void justHungUp()
{
    #ifdef DEBUG
    Serial.println("MAIN - Hung up.");
    #endif

    if ((clock.alarmState() == ALARM_TRIGGERED))
    {
        clock.snoozeAlarm(1);

        #ifdef DEBUG
        Serial.println("MAIN - Alarm snoozed.");
        #endif
    }

    // just in case...
    inputHelper.stopReading();
    speak.stopSpeaking();
    sound.stopPlaying();
}

void hungUp()
{
    if ((clock.alarmState() == ALARM_TRIGGERED) && (bell.state() == NOT_RINGING))
    {
        #ifdef DEBUG
        Serial.println("MAIN - Alarm triggered.");
        #endif

        bell.state(RINGING);
    }
}

void playStuff()
{
    if (speak.isSpeaking() && !sound.isPlaying())
    {
        sound.startPlaying(speak.soundBiteFolderNr(), speak.soundBiteFileNr());
        speak.nextSoundBite();
    }
}