class BELL
{
    public:
        BELL();

        void init();
        void check();

        void status(bool ringing);
        bool status();

    private:
        bool isRinging = false;
};