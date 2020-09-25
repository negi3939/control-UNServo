#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>

#include "filesave.h"

Filesave::Filesave(){
    f_n = "data/testdata.csv";
    fs.open(f_n.c_str(),std::ios::out);
	if(! fs.is_open()) {
		std::cout << "=======cannot open file=========="<<std::endl;
		exit(1);
	}
}

Filesave::Filesave(std::string l_f_n){
    f_n = l_f_n;
    fs.open(f_n.c_str(),std::ios::out);
	if(! fs.is_open()) {
		std::cout << "=======cannot open file=========="<<std::endl;
		exit(1);
	}
}

void Filesave::write_f(){
    fs << "ファイル出力　テスト" << std::endl;
}

Filesave::~Filesave(){
    fs.close();
}

#if defined(FILE_IS_MAIN)
int main(){    
    Filesave *fsa = new Filesave;
    fsa->write_f();
    delete fsa;
}
#endif