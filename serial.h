#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define FILE_NAME "data/log.dat"

class Serial{
    protected:
        int fd;
        unsigned char *buf;
        struct termios tio;              // シリアル通信設定
        int baudRate;                   //速度設定
        char *portname;                 //デバイスファイル名
        std::ofstream log;
    public:
        Serial();
        Serial(int baundrate);
        Serial(int baundrate,char *devname);
        int init();                     //ポートの初期化
        int kbhit();                    //キーボード割り込み入力の有無判定
        char getkey();                  //キーボード割り込み入力の取得 
        virtual int read_s();                   //読み取り
        virtual int write_s(std::string str);                  //書き込み
        int close_s();          
        ~Serial();        
};

#endif