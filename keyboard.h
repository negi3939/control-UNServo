#ifndef KBHIT_H
#define KBHIT_H

class keyboard{
    protected:
        struct termios init_tio;
    public:
        keyboard();
        ~keyboard();
        int kbhit();
        int getkey();
};

#endif