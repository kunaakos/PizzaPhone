class SOUND
{
    public:
        SOUND();

        void init();
        void check();

        void startPlaying(uint8_t folder, uint8_t file);
        void stopPlaying();
        boolean isPlaying();
 
    private:
        void sendCommand(uint8_t command, uint16_t dat, uint8_t feedback);
        boolean playing  = false;
        boolean gotFirstStopByte = false;
};