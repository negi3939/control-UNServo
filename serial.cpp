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


Serial::Serial(){
    baudRate = B9600;
    buf = new unsigned char[255];
    portname = new char[12];
    portname = "/dev/ttyS4";
    init();
}

Serial::Serial(int baundrate){
    baudRate = baundrate;
    buf = new unsigned char[255];
    init();
}
Serial::Serial(int baundrate,char *devname){
    buf = new unsigned char[255];
    int sizename = 30;
    portname = new char[sizename];
    for(int ii=0;ii<sizename;ii++){
        portname[ii] = devname[ii];
    }
    std::cout << portname << std::endl;
    baudRate = baundrate;
    init();
}

int Serial::init(){

    fd = open(portname, O_RDWR);
    if (fd < 0){
        std::cout<< "open error!" <<std::endl;
        return -1;
    }
    tio.c_cflag += CREAD;               // 受信有効
    tio.c_cflag += CLOCAL;              // ローカルライン（モデム制御なし）
    tio.c_cflag += CS8;                 // データビット:8bit
    tio.c_cflag += 0;                   // ストップビット:1bit
    tio.c_cflag += 0;                   // パリティ:None

    cfsetispeed( &tio, baudRate );   
    cfsetospeed( &tio, baudRate );

    cfmakeraw(&tio);                    // RAWモード

    tcsetattr( fd, TCSANOW, &tio );     // デバイスに設定を行う

    ioctl(fd, TCSETA, &tio);

    return 0;
}

int Serial::read_s(){
    int len,finishf=1;
    long count = 0;
    while(finishf) {
        len = read(fd, buf, sizeof(buf));   
        for(int ii = 0; ii < len; ii++) {
            std::cout << buf[ii] ;
            if(buf[ii]=='\n'){
                finishf = 0;
            }   
        }
    }
    std::cout << std::endl;
    return 0;
}


int Serial::write_s(std::string str){
    ssize_t ret = 0;
    int num = str.size();
    const char* buff = str.c_str();
    std::string strclear = "\r\n";
    const char* buffcl = strclear.c_str();
    ret = write(fd, buffcl, 2);
    ret = write(fd, buff, num);
    ret = write(fd, buffcl, 2);
    //std::cout << "send:" << buff << std::endl; 
    return ret;
}

int Serial::close_s(){
    close(fd);
    return 0;
}

Serial::~Serial(){
    close_s();
}

#if defined(SERIAL_IS_MAIN)
int main(){
    Serial *uniservo = new Serial;
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
    }
    uniservo->write_s(mvstop);
    //uniservo->read_s();
    delete uniservo;
    return 0;
}
#endif
