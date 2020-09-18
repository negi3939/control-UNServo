#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include "serial.h"

class contrServo{
    protected:
        Serial *uniservo;
    public:
        contrServo();
        contrServo(char *devname);
        void repeatedtorq();
        void zeropos();//原点へ移動
        ~contrServo();
};

contrServo::contrServo(){
    uniservo = new Serial;
}
contrServo::contrServo(char *devname){
    uniservo = new Serial(B9600,devname);
}

contrServo::~contrServo(){
    delete uniservo;
}

void contrServo::repeatedtorq(){
    std::string mvtcw = "MVT 1.0";
    std::string mvtccw = "MVT -1.0";
    std::string mvstop = "SV 0";
    long utime = 1000000; 
    for(int ii=0;ii<20;ii++){
        std::cout << "count: "<< ii << " ";
        uniservo->write_s(mvtcw);
        usleep(utime);
        std::cout << "count: "<< ii << " ";
        uniservo->write_s(mvtccw);
        usleep(utime);
        if(uniservo->getkey() == 'q'){//qを入力すると停止する
            break;
        };
    }
    uniservo->write_s(mvstop);
}

void contrServo::zeropos(){
    std::string mvzero = "MV 0.0";
    uniservo->write_s(mvzero);
}

int main(int argc, char *argv[]){
    contrServo *servo;
    if(argc>2){
        servo = new contrServo(argv[1]);
    }else{
        servo = new contrServo;
    }
    servo->zeropos();
    std::cout << "ready?(y)";
    while(getchar()!='y'){}
    servo->repeatedtorq();
    return 0;
}