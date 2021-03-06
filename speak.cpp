#include <Arduino.h>

#include "speak.h"
#include "SBDefines_HU.h"

#include "debug.h"

SPEAK::SPEAK()
{
    // nobody understands how constructors work in Arduino
}

void SPEAK::init()
{
    // nothing to init yet
}

void SPEAK::check()
{
	if (!speaking && queueIndex > 0)
	{
		startSpeaking();
	}
}

void SPEAK::stopSpeaking()
{
	speaking = false;
	queueIndex = 0;
}

void SPEAK::startSpeaking()
{
	speaking = true;
	playIndex = 0;
}

bool SPEAK::isSpeaking()
{
	return speaking;
}

void SPEAK::nextSoundBite()
{
	playIndex++;
	if (playIndex > QUEUE_LENGTH || playIndex >= queueIndex )
	{
		stopSpeaking();
	}
}

uint8_t SPEAK::soundBiteFolderNr()
{
	return (uint8_t)(queue[playIndex][0]);
}

uint8_t SPEAK::soundBiteFileNr()
{
	return (uint8_t)(queue[playIndex][1]);
}

bool SPEAK::addToQueue(uint8_t folderNr, uint8_t fileNr)
{
	#ifdef DEBUG
  if (DEBUG)
    {
      Serial.print("SPEAK - queuing: ");
      Serial.print(folderNr);
      Serial.print(" ");
      Serial.println(fileNr);
    }
	#endif

	if (queueIndex < QUEUE_LENGTH)
	{
		queue[queueIndex][0] = folderNr;
		queue[queueIndex][1] = fileNr;
		queueIndex++;
		return true; // added soundbite to queue;
	}
	else
	{
		return false; // queue full
	}
}

// Each of these methods queues a complete sentence

void SPEAK::currentTime(uint8_t hours, uint8_t minutes)
{
	#ifdef DEBUG
	Serial.println("SPEAK - queuing current time.");
	#endif

	// A pontos ido...
	addToQueue(MONDATRESZEK, A_PONTOS_IDO);
	sayTime(hours, minutes, false);
}

void SPEAK::alarmOff()
{
  addToQueue(MONDATRESZEK, EBRESZTOORA_NINCS_BEALLITVA);
  addToQueue(MONDATRESZEK, TARCSAZDBE_EBRESZTES);
}

void SPEAK::alarmSnoozed(uint8_t code)
{
  addToQueue(MONDATRESZEK, SZUNDI);
  promptForDeactivationCode(code);
}

void SPEAK::alarmSetTo(uint8_t hours, uint8_t minutes)
{
  addToQueue(MONDATRESZEK, AZ_EBRESZTOORA);
  sayTime(hours, minutes, true);
  addToQueue(MONDATRESZEK, VAN_BEALLITVA);
  addToQueue(MONDATRESZEK, UJ_EBRESZTES_IDOPONT);
}

void SPEAK::promptForDeactivationCode(uint8_t code)
{
  addToQueue(MONDATRESZEK, KIKAPCSOLASHOZ_TARCSAZD_BE);
  sayNumber(code);
  addToQueue(MONDATRESZEK, KODOT);
}

void SPEAK::message(uint8_t message)
{
  // THIS IS SOOO NOT FINAL
  switch(message) {
    case 11: // got code, alarm deactivated succesfully
      addToQueue(MONDATRESZEK, SIKERESEN_KIKAPCSOLTAD);
      break;
    case 12: // wrong code, try again with a new code
      addToQueue(MONDATRESZEK, HIBASKOD_PROBALKOZZUJRA_UJKODDAL);
      break;
  }

  #ifdef DEBUG
  Serial.println("SPEAK - playing err/succ msg");
  #endif
}

// These methods are sentence fragments

// reads time aloud
// ragozva is a Hungarian thing:
// "hh orara"/"hh ora mm percre" when true,
// "hh ora"/"hh ora mm perc" when false
void SPEAK::sayTime(uint8_t hours, uint8_t minutes, bool ragozva)
{
  sayNumber(hours, true);
  
  if (minutes == 0 & ragozva) 
  {
    addToQueue(MONDATRESZEK, SZO_ORARA);
  }
  else
  {
    addToQueue(MONDATRESZEK, SZO_ORA);
  }

  if (minutes > 0)
  {
    sayNumber(minutes, true);
    if (ragozva)
    {
      addToQueue(MONDATRESZEK, SZO_PERCRE);
    }
    else
    {
      addToQueue(MONDATRESZEK, SZO_PERC);
    }
  }
}


void SPEAK::sayNumber (uint8_t number)
{
  sayNumber(number, false);
}

// reads number < 60 aloud
// szamnev is a Hungarian thing:
// 2 is "ket" when true, "ketto" when false
void SPEAK::sayNumber (uint8_t number, bool szamnev)
{
  if (number < 10)
  {
    if (!szamnev & (number == 2))
    {
      addToQueue(EGYESEK, 12); // "ketto"
    }
    else
    {
      addToQueue(EGYESEK, number); // "ket" and the rest
    }
  }
  else if ((number % 10) == 0)
  {
    addToQueue(TIZESEK, number / 10);
  }
  else
  {
    addToQueue(TIZENESEK, number / 10);
    if (!szamnev & ((number % 10) == 2))
    {
      addToQueue(EGYESEK, 12); // "ketto"
    }
    else
    {
      addToQueue(EGYESEK, number % 10); // "ket" and the rest
    }
  }
}