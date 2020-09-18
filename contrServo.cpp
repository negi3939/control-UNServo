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

#include "serial.h"

class contrServo : public Serial{
    protected:
        //Serial *uniservo;
        std::fstream fs;
    public:
        contrServo();
        contrServo(char *devname);
        void init();
        void repeatedtorq(int num,double torq);//繰り返す
        void zeropos();//原点へ移動
        void readpos();
        ~contrServo();
};

contrServo::contrServo(){
    //uniservo = new Serial;
    init();
}
contrServo::contrServo(char *devname) : Serial(B9600,devname){
    //uniservo = new Serial(B9600,devname);
    init();
}

contrServo::~contrServo(){
    //delete uniservo;
}

void contrServo::init(){
    std::stringstream filename;
    filename << "data/testdata.dat";//保存するデータのなまえ
    std::string f_n = filename.str();
    fs.open(f_n.c_str(),std::ios::out);
	if(! fs.is_open()) {
		std::cout << "=======cannot open file=========="<<std::endl;
		exit(1);
	}
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
        write_s(mvtcw);
        usleep(0.5*utime);
        readpos();
        usleep(0.5*utime);
        std::cout << "count: "<< ii << " ";
        write_s(mvtccw);
        usleep(0.5*utime);
        readpos();
        usleep(0.5*utime);
        if(getkey() == 'q'){//qを入力すると停止する
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
    while(finishf){
        std::cout << " ================== choose mode =============== " << std::endl;
        std::cout << "\t 'r':repeatd test mode" << std::endl;
        std::cout << "\t 'q':quit" << std::endl;
        std::cout << "input->";
        switch (getchar()){
        case 'r':
            std::cout << "ready?(y)";
            while(getchar()!='y'){}
            servo->repeatedtorq(count,torq);   
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