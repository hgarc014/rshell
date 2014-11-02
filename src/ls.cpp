#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <stdio.h>

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
    if(dirp == NULL){
        perror("opendir");
        exit(1);
    }
    dirent *direntp;
    while ((direntp = readdir(dirp))){
        if(direntp == NULL){
            perror("readdir");
            exit(1);
        }
        cout << direntp->d_name << endl;
        if(direntp->d_name[0] != '.')
            lsL(direntp->d_name);

    }
    closedir(dirp);
    cout << endl;

    return 0;
}

void lsL(const string & file){

    struct stat statbuf;
    cout << "executing=" << file << "|" << endl;
    if(stat(file.c_str(), &statbuf) == -1){
        perror("stat");
        exit(1);
    }

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
    time = " " + time.substr(0, time.size()-9) + " ";
    struct passwd *usr = getpwuid(statbuf.st_uid);
    struct group *gp = getgrgid(statbuf.st_gid);
    if(gp == NULL){
        perror("getGroup");
        exit(1);
    }
    if(usr == NULL){
        perror("getUSR");
        exit(1);
    }
    cout << " " << usr->pw_name << " " << gp->gr_name << " "<< statbuf.st_size << time << file << endl;

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
