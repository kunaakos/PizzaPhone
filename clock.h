#define ALARM_OFF 0
#define ALARM_ACTIVE 1
#define ALARM_SNOOZED 2
#define ALARM_TRIGGERED 3

class CLOCK
{
    public:
        CLOCK();

        void init();
        void check();

        void setAlarm(uint8_t hours, uint8_t minutes);
        void unSetAlarm();
        void snoozeAlarm(uint8_t minutes);
        void alarmTime(uint8_t hours, uint8_t minutes);
        time_t alarmTime();
        uint8_t alarmState();

    private:
        uint8_t currentAlarmState = 0;
        void alarmState(bool newState);
        uint8_t bcd2dec(uint8_t n);
};