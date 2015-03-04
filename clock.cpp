#include <Arduino.h>

#include <DS3232RTC.h> // https://github.com/JChristensen/DS3232RTC
#include <Time.h>     
#include <Wire.h>

#include "clock.h"

CLOCK::CLOCK()
{
    // nobody understands how constructors work in Arduino
}

void CLOCK::init()
{
	setSyncProvider(RTC.get);
}

void CLOCK::check()
{
	// nothing to do in the main loop yet
}

void CLOCK::setAlarm(uint8_t hours, uint8_t minutes)
{
	RTC.setAlarm(ALM2_MATCH_HOURS, minutes, hours, 0);
}

time_t CLOCK::getAlarm()
{
	return RTC.getAlarm(ALARM_2);
}