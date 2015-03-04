class DIAL
{
    public:
        DIAL();

        void init();
        void check();

        boolean available();
        byte lastNumber();

    private:
        boolean numberAvailable = false;
};