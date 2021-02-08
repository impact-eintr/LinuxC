#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

char *filename = nullptr;

void exit_(){
    if(chmod(filename,0755) < 0){
        perror("chmod");
        exit(1);
    }
}

int main(int argc,char **argv){

    if(argc <3){
        fprintf(stderr,"Usage...\n");
        exit(1);
    }
    std::ifstream infile(argv[1],std::ifstream::binary);

    filename = argv[2];
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
