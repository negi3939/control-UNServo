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
        void repeatedtorq(int num,double torq);//繰り返す
        void zeropos();//原点へ移動
        void readpos();
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

void contrServo::repeatedtorq(int num,double torq){
    char *tqch;
    sprintf(tqch,"%.3f",torq);
    std::string tqst(tqch);
    std::string mvtcw = "MVT ";
    std::string mvtccw = "MVT -";
    mvtcw += tqst;
    mvtccw += tqst;
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
    //uniservo->write_s(mvzero);
}

int main(int argc, char *argv[]){
    contrServo *servo;
    int finishf = 1;
    if(argc>2){
        servo = new contrServo(argv[1]);
    }else{
        servo = new contrServo;
    }
    servo->zeropos();
    while(finishf){
        std::cout << " ================== choose mode =============== " << std::endl;
        std::cout << "\t 'r':repeatd test mode" << std::endl;
        std::cout << "\t 'q':quit" << std::endl;
        std::cout << "input->";
        switch (getchar()){
        case 'r':
            std::cout << "ready?(y)";
            while(getchar()!='y'){}
            servo->repeatedtorq(10,1.0d);   
            break;
        case 'q':
            finishf = 0;   
            break;
        default:
            break;
        }
    }
    return 0;
}