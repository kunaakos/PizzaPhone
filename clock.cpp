#include <Arduino.h>

#include <DS3232RTC.h> // https://github.com/JChristensen/DS3232RTC
#include <Time.h>     
#include <Wire.h>

#include "clock.h"

#include "debug.h"

CLOCK::CLOCK()
{
    // nobody understands how constructors work in Arduino
}

void CLOCK::init()
{
    setSyncProvider(RTC.get);

    // read alarm state from RTC
    currentAlarmState = RTC.readRTC(RTC_CONTROL);
    currentAlarmState = (currentAlarmState & _BV(A2IE)) >> A2IE;

    #ifdef DEBUG
    Serial.print("CLOCK - read A2IE: ");
    Serial.println(currentAlarmState);
    #endif
}

void CLOCK::check()
{
    if ( RTC.alarm(ALARM_2) ) {
        alarmState(ALARM_TRIGGERED);
    }
}

void CLOCK::setAlarm(uint8_t hours, uint8_t minutes)
{
    alarmTime(hours, minutes);
    RTC.alarmInterrupt(ALARM_2, true);
    alarmState(ALARM_ACTIVE);
}

void CLOCK::unSetAlarm()
{
    RTC.alarmInterrupt(ALARM_2, false);
    alarmState(ALARM_OFF);
}

void CLOCK::snoozeAlarm(uint8_t minutesToSnooze)
{
    uint8_t minutes = minute();
    uint8_t hours = hour();

    minutes += minutesToSnooze;

    if (minutesToSnooze >= 59)
    {
        minutes -= 60;

        if (hours >= 23)
        {
            hours = 0;
        }
        else
        {
            hours++;
        }
    }

    alarmTime(hours, minutes);
    alarmState(ALARM_SNOOZED);
}

void CLOCK::alarmTime(uint8_t hours, uint8_t minutes)
{
    #ifdef DEBUG
    Serial.print("CLOCK - setting alarm to: ");
    Serial.print(hours);
    Serial.print(" ");
    Serial.println(minutes);
    #endif

    RTC.setAlarm(ALM2_MATCH_HOURS, minutes, hours, 0);
}

time_t CLOCK::alarmTime()
{
    tmElements_t tmE;
    uint8_t values[3];

    RTC.readRTC(ALM2_MINUTES, values, 2);

    tmE.Second = 0; // ALARM_2 doesn't store seconds
    tmE.Minute = bcd2dec(values[0] & ~_BV(7));  // bit 7 is always an alarm status register!
    tmE.Hour = bcd2dec(values[1] & ~_BV(HR1224) & ~_BV(7));    //assumes 24hr clock

    // these will need real values, but fuck them for now
    tmE.Wday = weekday();
    tmE.Day = day();
    tmE.Month = month();
    tmE.Year = year() - 1970;

    #ifdef DEBUG
    time_t tm = makeTime(tmE);
    Serial.print("CLOCK - read alarm settings: ");
    Serial.print(hour(tm));
    Serial.print(":");
    Serial.print(minute(tm));
    Serial.print(":");
    Serial.println(second(tm));
    #endif

    return makeTime(tmE);
    // return RTC.getAlarm(ALARM_2); // FUCK THIS THING
}

void CLOCK::alarmState(bool newState)
{
    currentAlarmState = newState;
}

uint8_t CLOCK::alarmState()
{
    return currentAlarmState;
}

uint8_t CLOCK::bcd2dec(uint8_t n)
{
    return n - 6 * (n >> 4);
}
