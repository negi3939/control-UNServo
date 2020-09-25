#ifndef _SERIAL_H_
#define _SERIAL_H_

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
        virtual int read_s();                   //読み取り
        virtual int write_s(std::string str);                  //書き込み
        int close_s();          
        ~Serial();        
};

#endif