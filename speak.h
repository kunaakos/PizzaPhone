#define QUEUE_LENGTH 20
#define ERRORMSG_GENERIC_SHORT 1

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
        void alarmOff();
        void alarmSnoozed(uint8_t code);
        void alarmSetTo(uint8_t hours, uint8_t minutes);
        void promptForDeactivationCode(uint8_t code);
        void errorMessage(uint8_t error);
        void successMessage(uint8_t message);
        void sayNumber(uint8_t number);

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