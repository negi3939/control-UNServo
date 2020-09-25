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


#include "keyboard.h"
#include "serial.h"
#include "filesave.h"

class contrServo : public Serial , public Filesave{
    protected:
    public:
        contrServo();
        contrServo(char *devname);
        contrServo(std::string l_f_n);
        contrServo(char *devname,std::string l_f_n);
        void repeatedtorq(int num,double torq);//繰り返す
        void zeropos();//原点へ移動
        void readpos();
        int read_s() override;
        ~contrServo();
};

contrServo::contrServo() : Serial(),Filesave(){}
contrServo::contrServo(char *devname) : Serial(B9600,devname),Filesave() {}
contrServo::contrServo(std::string l_f_n) : Serial(),Filesave(l_f_n){}
contrServo::contrServo(char *devname,std::string l_f_n) : Serial(),Filesave(l_f_n){}
contrServo::~contrServo(){}


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
        write_s(mvtcw);
        usleep(0.5*utime);
        fs << ii << ","<< std::flush;
        readpos();
        fs << std::endl;
        usleep(0.5*utime);
        std::cout << "count: "<< ii << " ";
        write_s(mvtccw);
        usleep(0.5*utime);
        fs << ii << ","<< std::flush;
        readpos();
        fs << std::endl;
        usleep(0.5*utime);
        if(getkey() == 'q'){//qを入力すると停止する
            finish_inturkey();
            break;
        };
    }
    write_s(mvstop);
}

void contrServo::zeropos(){
    std::string mvzero = "SV 0";
    std::string poszero = "POSORG";
    write_s(mvzero);
    write_s(poszero);
}

void contrServo::readpos(){
    std::string askpos = "A?";
    write_s(askpos);
    read_s();
}

int contrServo::read_s(){
    int len,finishf=1;
    long count = 0;
    while(finishf) {
        len = read(fd, buf, sizeof(buf));   
        for(int ii = 0; ii < len; ii++) {
            std::cout << buf[ii] << std::flush;
            if((buf[ii]!='\r')&&(buf[ii]!='\n')&&(buf[ii]!='?')&&(buf[ii]!='A')&&(buf[ii]!='=')){
                fs << buf[ii] << std::flush;
            }
            if(buf[ii]=='\n'){
                finishf = 0;
            }   
        }
    }
    std::cout << std::endl;
    return 0;
}

#if defined(CONTROL_IS_MAIN)
int main(int argc, char *argv[]){
    contrServo *servo;
    int finishf = 1;
    int count = 20;
    double torq = 1.0d;
    if(argc>2){
        servo = new contrServo(argv[1]);
    }else{
        servo = new contrServo;
    }
    servo->zeropos();
    char ch[20];
    while(finishf){
        std::cout << " ================== choose mode =============== " << std::endl;
        std::cout << "\t 'c':constant velocity mode" << std::endl;
        std::cout << "\t 'r':repeatd test mode" << std::endl;
        std::cout << "\t 'q':quit" << std::endl;
        std::cout << "input--------->";
        std::cin >> ch;
        switch (ch[0]){
            case 'c':
                break;
            case 'r':
                std::cout << "input torque--->";
                std::cin >> torq;
                std::cout << "input number of repetitions--->";
                std::cin >> count;
                std::cout << "torq is " << torq << std::endl;
                std::cout << "number is " << count << std::endl;
                std::cout << "ready?(y)";
                std::cin >> ch;
                while(ch[0]!='y'){}
                servo->repeatedtorq(count,torq);
                finishf = 0;   
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

#endif