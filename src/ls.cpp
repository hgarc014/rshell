#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include <iostream>

using namespace std;

#define FLAG_a 1
#define FLAG_l 2
#define FLAG_R 4

#define eldash else cout << "-"

int main(int argc, char*argv[]){

    int flags = 0;

    for(int i =1; i < argc; ++i){
        if(argv[i][0] == '-'){
            for(int j = 1; argv[i][j] != 0 && argv[i][j] != ' ';j++){
                if(argv[i][j] == 'a')
                    flags |= FLAG_a;
                else if(argv[i][j] == 'l')
                    flags |= FLAG_l;
                else if(argv[i][j] == 'R')
                    flags |= FLAG_R;
            }
        }
    }



    return 0;
}

void lsL(){

    struct stat statbuf;
    stat(".", &statbuf);

    if(statbuf.st_mode & S_IRUSR)
        cout << "r";
    eldash;
    if(statbuf.st_mode & S_IWUSR)
        cout << "w";
    eldash;
    if(statbuf.st_mode & S_IXUSR)
        cout << "x";
    eldash;
    if(statbuf.st_mode & S_IRGRP)
        cout << "r";
    eldash;



}

void lsA(){

}

void lsR(){

}
