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


Serial::Serial(){
    baudRate = B9600;
    buf = new unsigned char[255];
    portname = new char[12];
    portname = "/dev/ttyACM0";
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
    log.open(FILE_NAME, std::ios::trunc);
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

    ioctl(fd, TCSETS, &tio);

    return 0;
}

int Serial::read_s(){
    read(fd, buf, sizeof(buf));
    int len,finishf=1;
    long count = 0;
    while(finishf) {
        len = read(fd, buf, sizeof(buf));   
        for(int ii = 0; ii < len; ii++) {
                    std::cout << buf[ii] ;
        }   
        if (kbhit()){
            std::cout << "finish" << std::endl;
			finishf = 0;
		}
    }
    std::cout << std::endl;
	std::cout << "Stop recieving and sending" << std::endl;
    return 0;
}

int Serial::kbhit(void){
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int Serial::write_s(){
    ssize_t num = 0;
    char buff[10]="MVT 1.0\r\n";
    std::cout << std::flush;
    num = write(fd, &buff, 10);
    return num;
}

int Serial::close_s(){
    close(fd);
    return 0;
}

Serial::~Serial(){
    close_s();
}

#if 1
int main(){
    Serial *arduino = new Serial;
    arduino->write_s();
    arduino->read_s();
    delete arduino;
    return 0;
}
#endif