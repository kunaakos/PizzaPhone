class BELL
{
    public:
        BELL();

        void init();
        void check();

        void state(bool ringing);
        bool state();

    private:
        bool isRinging = false;
};