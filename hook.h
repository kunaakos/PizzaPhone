#define HUNG_UP 0
#define PICKED_UP 1
#define JUST_HUNG_UP 2
#define JUST_PICKED_UP 3

class HOOK
{
    public:
        HOOK();

        void init();
        void check();

        uint8_t state();

    private:
        uint8_t stateToReturn = 0; // 0 hung up, 1 picked up, 2 just hung up, 3 just picked up 
        uint8_t debouncedState = 0;
        uint8_t currentState = 0;
        uint8_t lastState = 0;
        long lastDebounceTime = 0;
        long debounceDelay = 50;
};