class CLOCK
{
    public:
        CLOCK();

        void init();
        void check();

        void setAlarm(uint8_t hours, uint8_t minutes);
        time_t getAlarm();
};