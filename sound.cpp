#include <Arduino.h>
#include <SoftwareSerial.h>

#include "sound.h"

#define CMD_SEL_DEV 0X09
#define DEV_TF 0X02
#define CMD_SET_VOLUME 0X06
#define CMD_PLAY_FOLDER_FILENAME 0X0F
#define CMD_STOP 0X16

#define DEBUG

SoftwareSerial soundSerial(5,6);

SOUND::SOUND()
{
    // nobody understands how constructors work in Arduino
}

void SOUND::init()
{   
    soundSerial.begin(9600);
    delay(200); // wait until chip initialization is complete
    sendCommand(CMD_SEL_DEV, DEV_TF, false); // select the TF card
    sendCommand(CMD_SET_VOLUME, 30, false);
    delay(200); // wait for 200ms
}

void SOUND::check()
{
    while (soundSerial.available())
    {

        // this byte signals that playback is finished
        // need to get this twice for some reason
        // datasheet in chinese only...
        // the feedback msg is 10 bytes long
        // but no point in processing all that 
        if(soundSerial.read() == 0x3D) 
        {
            if (gotFirstStopByte) 
            {
                playing = false;
                gotFirstStopByte = false;
            } 
            else 
            {
                gotFirstStopByte = true;
            }
        }
    }
}

void SOUND::startPlaying(uint8_t folder, uint8_t file)
{
    #ifdef DEBUG
    Serial.print("SOUND - playing soundbite from queue: ");
    Serial.print(folder);
    Serial.print(" ");
    Serial.println(file);
    #endif
    uint16_t folderFile = (folder << 8) | file;
    sendCommand( CMD_PLAY_FOLDER_FILENAME , folderFile, true);
    playing = true;
}

void SOUND::stopPlaying()
{
    sendCommand(CMD_STOP, 0, false);
    playing = false; // check feedback byte for stop command!
}

boolean SOUND::isPlaying()
{
    return playing;
}

void SOUND::sendCommand(uint8_t command, uint16_t dat, uint8_t feedback)
{
    static int8_t Send_buf[8] = {0};

    delay(20);
    Send_buf[0] = 0x7e; // starting byte
    Send_buf[1] = 0xff; // version
    Send_buf[2] = 0x06; // the number of bytes of the command without starting byte and ending byte
    Send_buf[3] = command;
    Send_buf[4] = feedback;// 0x00 = no feedback, 0x01 = feedback
    Send_buf[5] = (uint8_t)(dat >> 8);// datah
    Send_buf[6] = (uint8_t)(dat); // datal
    Send_buf[7] = 0xef; // ending byte
    for(uint8_t i=0; i<8; i++)
    {
      soundSerial.write(Send_buf[i]) ;
    }
}