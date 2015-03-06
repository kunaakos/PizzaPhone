#define QUEUE_LENGTH 20

class SPEAK
{
    public:
        SPEAK();

        void init();
        void check();

        void stopSpeaking();
        bool isSpeaking();
        void nextSoundBite();
        uint8_t soundBiteFolderNr();
        uint8_t soundBiteFileNr();

        void currentTime(uint8_t hours, uint8_t minutes);
        void alarmSettings();
        void alarmSettings(uint8_t hours, uint8_t minutes);
        void promptForNewAlarmSettings();
        void errorMessage(uint8_t error);

    private:
        void startSpeaking();
        bool addToQueue(uint8_t folderNr, uint8_t fileNr);
        void sayTime(uint8_t hours, uint8_t minutes, bool ragozva);
        void sayNumber(uint8_t number, bool szamnev);

        bool speaking = false;
        uint8_t queue[QUEUE_LENGTH][2];
        uint8_t queueIndex = 0;
        uint8_t playIndex = 0;
};