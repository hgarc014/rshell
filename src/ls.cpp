#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>

using namespace std;

#define FLAG_a 1
#define FLAG_l 2
#define FLAG_R 4

#define eldash else cout << "-"


void lsL(const string & file);

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

    string s = ".";
    DIR *dirp = opendir(s.c_str());
    dirent *direntp;
    while ((direntp = readdir(dirp)))
        lsL(direntp->d_name);  // use stat here to find attributes of file
    closedir(dirp);
    cout << endl;

    return 0;
}

void lsL(const string & file){

    struct stat statbuf;
    stat(file.c_str(), &statbuf);

    if(S_ISDIR(statbuf.st_mode))
        cout << "d";
    eldash;

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
    if(statbuf.st_mode & S_IWGRP)
        cout << "w";
    eldash;
    if(statbuf.st_mode & S_IXGRP)
        cout << "x";
    eldash;
    if(statbuf.st_mode & S_IROTH)
        cout << "r";
    eldash;
    if(statbuf.st_mode & S_IWOTH)
        cout << "w";
    eldash;
    if(statbuf.st_mode & S_IXOTH)
        cout << "x";
    eldash;
    string time = ctime(&statbuf.st_ctime);
    time = " "+ time.substr(0, time.size()-9) + " ";
    cout << " user group " << statbuf.st_size << time << file << endl;

    cout << "user=" << statbuf.st_uid << endl
        << "group=" << statbuf.st_gid << endl;
//    cout << endl << "IDfile=" << statbuf.st_dev << endl
//        << "inode=" << statbuf.st_ino << endl
//        << "nlink=" << statbuf.st_nlink << endl
//        << "userID=" << statbuf.st_uid << endl
//        << "groupID=" << statbuf.st_gid << endl
//        << "deviceID=" << statbuf.st_rdev << endl
//        << "blksz=" << statbuf.st_blksize << endl
//        << "blkCnt=" << statbuf.st_blocks << endl
//        << "lastAcc=" << ctime(&statbuf.st_atime) << endl
//        << "lastMod=" << ctime(&statbuf.st_mtime) << endl
//        << "lastChg=" << ctime(&statbuf.st_ctime) << endl;
    cout << endl;
}

void lsA(){

}

void lsR(){

}
