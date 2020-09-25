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
#include <sys/times.h>
#include <sys/time.h>

#include "keyboard.h"
#include "serial.h"
#include "filesave.h"

class contrServo : public Serial , public Filesave , public keyboard{
    protected:
        struct timeval start_time,init_time,end_time;
        long seconds,useconds;
        double getabstime();
        double getreltime();
    public:
        contrServo();
        contrServo(char *devname);
        contrServo(std::string l_f_n);
        contrServo(char *devname,std::string l_f_n);
        void repeatedtorq(int num,double torq);//繰り返す
        void constvelocity(double vel,double l_tim);
        void consttorq(double torq,double l_tim);
        void clearerror();
        void zeropos();//原点へ移動
        void readpos();
        int read_s() override;
        ~contrServo();
};

contrServo::contrServo() : Serial(),Filesave(),keyboard(){}
contrServo::contrServo(char *devname) : Serial(B9600,devname),Filesave(),keyboard() {}
contrServo::contrServo(std::string l_f_n) : Serial(),Filesave(l_f_n),keyboard(){}
contrServo::contrServo(char *devname,std::string l_f_n) : Serial(),Filesave(l_f_n),keyboard(){}
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
    double utime = 0.50d;
    gettimeofday(&start_time, NULL); 
    for(int ii=0;ii<num;ii++){
        std::cout << "count: "<< ii << " ";
        write_s(mvtcw);
        gettimeofday(&init_time, NULL);
        while(getreltime()<0.5d*utime){
            fs << getabstime() << "," << ii << ","<< std::flush;
            readpos();
            fs << std::endl;
        }
        std::cout << "count: "<< ii << " ";
        write_s(mvtccw);
        gettimeofday(&init_time, NULL);
        while(getreltime()<0.5d*utime){
            fs << getabstime() << "," << ii << ","<< std::flush;
            readpos();
            fs << std::endl;
        }
        if(getkey() == 'q'){//qを入力すると停止する
            finish_inturkey();
            break;
        };
    }
    write_s(mvstop);
}

void contrServo::constvelocity(double vel,double l_tim){
    long tim_unit = 10;
    char *velch;
    sprintf(velch,"%.3f",vel);
    std::string mvv = "MVV ";
    std::string mvstop = "SV 0";
    std::string velst(velch);
    mvv += velst;
    write_s(mvv);
    gettimeofday(&init_time, NULL);
    while(getreltime()<l_tim){
        if(getkey() == 'q'){//qを入力すると停止する
            finish_inturkey();
            break;
        };
    }
    write_s(mvstop);
}

void contrServo::consttorq(double torq,double l_tim){
    long tim_unit = 10;
    char *torqch;
    sprintf(torqch,"%.3f",torq);
    std::string mvt = "MVT ";
    std::string mvstop = "SV 0";
    std::string torqst(torqch);
    mvt += torqst;
    write_s(mvt);
    gettimeofday(&init_time, NULL);
    while(getreltime()<l_tim){
        if(getkey() == 'q'){//qを入力すると停止する
            finish_inturkey();
            break;
        };
    }
    write_s(mvstop);
}

void contrServo::clearerror(){
    std::string cancelst = "SVERCLR";
    write_s(cancelst);
    sleep(6);
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
            //std::cout << buf[ii] << std::flush;
            if((buf[ii]>45)&&(buf[ii]<58)){
                fs << buf[ii] << std::flush;
            }
            if(buf[ii]=='\n'){
                finishf = 0;
            }   
        }
    }
    //std::cout << std::endl;
    return 0;
}

double contrServo::getabstime(){
        gettimeofday(&end_time, NULL);
		seconds = end_time.tv_sec - start_time.tv_sec; //seconds
   		useconds = end_time.tv_usec - start_time.tv_usec; //milliseconds
		double time = (double)((seconds) + useconds/1000000.0);
        return time;
};

double contrServo::getreltime(){
        gettimeofday(&end_time, NULL);
		seconds = end_time.tv_sec - init_time.tv_sec; //seconds
   		useconds = end_time.tv_usec - init_time.tv_usec; //milliseconds
		double time = (double)((seconds) + useconds/1000000.0);
        return time;
};

#if defined(CONTROL_IS_MAIN)
int main(int argc, char *argv[]){
    contrServo *servo;
    int finishf = 1;
    int count = 20;
    double torq = 1.0d;
    double velocity = 1.0d;
    double t_tim;
    if(argc>1){
        servo = new contrServo(argv[1]);
    }else{
        servo = new contrServo;
    }
    servo->zeropos();
    char ch[20];
    while(finishf){
        std::cout << " ================== choose mode =============== " << std::endl;
        std::cout << "\t 'v':constant velocity mode" << std::endl;
        std::cout << "\t 't':constant torque mode" << std::endl;
        std::cout << "\t 'r':repeatd test mode" << std::endl;
        std::cout << "\t 'c':clear error" << std::endl;
        std::cout << "\t 'q':quit" << std::endl;
        std::cout << "input--------->";
        std::cin >> ch;
        switch (ch[0]){
            case 'v':
                std::cout << "input velocity--->";
                std::cin >> velocity;
                std::cout << "input running time--->";
                std::cin >> t_tim;
                std::cout << "velocity is " << velocity << std::endl;
                std::cout << "running time is " << t_tim << std::endl;
                std::cout << "ready?(y)";
                std::cin >> ch;
                while(ch[0]!='y'){}
                servo->constvelocity(velocity,t_tim);
                finishf = 0;   
                break;
            case 't':
                std::cout << "input torque--->";
                std::cin >> torq;
                std::cout << "input running time--->";
                std::cin >> t_tim;
                std::cout << "torq is " << torq << std::endl;
                std::cout << "running time is " << t_tim << std::endl;
                std::cout << "ready?(y)";
                std::cin >> ch;
                while(ch[0]!='y'){}
                servo->consttorq(torq,t_tim);
                finishf = 0;   
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
            case 'c':
                servo->clearerror();
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