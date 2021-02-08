#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

char *filename = nullptr;

void exit_(){
    if(chmod(filename,0755) < 0){
        perror("chmod");
        exit(1);
    }
}
void copy(char *src){
    std::ifstream infile(src,std::ifstream::binary);

    std::ofstream outfile(filename,std::ofstream::binary);

    infile.seekg(0,infile.end);
    long size = infile.tellg();
    infile.seekg(0);

    char* buffer = new char[size];
    infile.read(buffer,size);

    outfile.write(buffer,size);

    delete[] buffer;

    outfile.close();
    infile.close();
    atexit(exit_);
}
int main(int argc,char **argv){

    if(argc <3){
        fprintf(stderr,"Usage...\n");
        exit(1);
    }

    pid_t pid;
    pid = fork();
    if(pid < 0){
        perror("fork");
    }
    else if(pid == 0){
        filename = argv[2];
        copy(argv[1]);
        sleep(1);
        execl("/usr/bin/mpg123","mpg123",filename,NULL);
    }
    else{
        wait(NULL);
        std::cout<<"程序结束"<<std::endl;
        exit(0);
    }

}
