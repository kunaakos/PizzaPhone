#define CODE_DIGITS 2

#define INACTIVE 0
#define READING_CODE 10
#define CODE_OK 11
#define CODE_INCORRECT 12
#define READING_TIME 20
#define TIME_OK 21
#define TIME_INCORRECT 22
#define READING_NUMBER 30
#define NUMBER_OK 31

class INPUTHELPER
{
    public:
        INPUTHELPER();
        
        void init();
        void check();

        uint8_t status();
        void stopReading();
        uint8_t addDigit(uint8_t digit);
        void startReadingTime();
        time_t getTime();
        uint8_t startReadingCode();
        uint8_t startReadingNumber(uint8_t max);
        uint8_t getNumber();
        bool isReading();
        bool hasResult();

    private:
        uint8_t code[CODE_DIGITS];
        uint8_t codeIndex;
        uint8_t number;
        uint8_t numberMaxVal;
        uint8_t timeDigits[4];
        uint8_t timeDigitsIndex;
        uint8_t timeHours;
        uint8_t timeMinutes;
        uint8_t currentStatus = 0;

        uint8_t validateNumber(uint8_t digit);
        uint8_t validateCode(uint8_t digit);
        uint8_t validateTime(uint8_t digit);
};