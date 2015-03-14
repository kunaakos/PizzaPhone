#include <Arduino.h>
#include <Time.h>     

#include "inputHelper.h"

#include "debug.h"

INPUTHELPER::INPUTHELPER()
{
    // nobody understands how constructors work in Arduino
}

void INPUTHELPER::init()
{
    // nothing to init yet
}

void INPUTHELPER::check()
{
    // nothing to do in main loop
}

uint8_t INPUTHELPER::status()
{
    return currentStatus;
}

void INPUTHELPER::stopReading()
{
    currentStatus = INACTIVE;
}

uint8_t INPUTHELPER::addDigit(uint8_t digit)
{
    switch (currentStatus) {
        case READING_CODE:
                return validateCode(digit);
            break;
        case READING_NUMBER:
                return validateNumber(digit);
            break;
        case READING_TIME:
                return validateTime(digit);
            break;
        default:
            return 255;
            break;
    }
}

uint8_t INPUTHELPER::validateNumber(uint8_t digit)
{

}

uint8_t INPUTHELPER::validateCode(uint8_t digit)
{
    if (code[codeIndex] == digit)
    {
        if (++codeIndex == CODE_DIGITS)
        {
            currentStatus = CODE_OK;
        }
        return digit;
    }
    else
    {
        currentStatus = CODE_INCORRECT;
        return 255; // err
    }
}

uint8_t INPUTHELPER::validateTime(uint8_t digit)
{

}


void INPUTHELPER::startReadingTime()
{
    // reset index and set status
    timeDigitsIndex = 0;
    currentStatus = READING_TIME;
}

time_t INPUTHELPER::getTime()
{
    // l8rz
}

uint8_t INPUTHELPER::startReadingCode()
{
    // generate a deactivation code
    for (int i = 0; i < (CODE_DIGITS); ++i)
    {
        code[i] = random(9) + 1;
    }
    
    // reset index and set status
    codeIndex = 0;
    currentStatus = READING_CODE;
    
    // return code
    uint8_t toReturn = 0;
    for (int i = 0; i < (CODE_DIGITS); ++i)
    {
        toReturn = toReturn * 10 + code[i];
    }

    #ifdef DEBUG
    Serial.print("INPUTHELPER - generated and returned: ");
    Serial.println(toReturn);
    #endif

    return toReturn;
}

uint8_t INPUTHELPER::startReadingNumber(uint8_t max)
{
    // l8rz
}

uint8_t INPUTHELPER::getNumber()
{
    // l8rz
}

bool INPUTHELPER::isReading()
{
    return (currentStatus > 0) && (currentStatus % 10 == 0);
}

bool INPUTHELPER::hasResult()
{

    return (currentStatus > 0) && (currentStatus % 10 != 0);
}
