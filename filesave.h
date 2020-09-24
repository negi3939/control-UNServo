#ifndef _FILE_SAVE_H_
#define _FILE_SAVE_H_

class Filesave{
    protected:
        std::string f_n;
        std::fstream fs;
    public:
        Filesave();
        Filesave(std::string l_f_n);
        virtual void write_f();
        ~Filesave();
};


#endif
