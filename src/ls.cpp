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
#include <algorithm>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define FLAG_a 1
#define FLAG_l 2
#define FLAG_R 4

#define eldash else cout << "-"
#define checkA(x) (x & FLAG_a)
#define checkL(x) (x & FLAG_l)
#define checkR(x) (x & FLAG_R)


void lsL(const string & file);
void checkinput(const int, const string &s);
bool noCaseComp(const string &one, const string &two);

int main(int argc, char*argv[]){

    int flags = 0;
    vector<string> input;

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
        else{
            input.push_back(argv[i]);
        }
    }

    sort(input.begin(), input.end(), noCaseComp);

    cout << "---------outputing sorted--------" << endl;
    for(unsigned i = 0; i < input.size(); ++i){
    cout << input.at(i) << endl;
    }
    cout << "-------done outputing sorted-------" << endl;

    string s = ".";
    checkinput(flags,s);
    cout << endl;
    return 0;
}

void lsL(const string & file){

    struct stat statbuf;
    //    cout << "executing=" << file << "|" << endl;
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
    time = time.substr(3, time.size()-9) + " ";
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
    cout << " " << usr->pw_name << " " << gp->gr_name << " "<< statbuf.st_size << time << file;

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

void lsR(){

}

void checkinput(const int flags, const string &s){
    DIR *dirp = opendir(s.c_str());
    if(dirp == NULL){
        perror("opendir");
        exit(1);
    }
    dirent *direntp;
    vector<string> file;
while((direntp = readdir(dirp))){
    if(direntp == NULL){
        perror("readdir");
        exit(1);
    }
    file.push_back(direntp->d_name);
}
sort(file.begin(),file.end(),noCaseComp);

for(unsigned i=0; i < file.size(); ++i){
        if(checkL(flags) && checkA(flags) && checkR(flags)){
            cout << "all flags were passed in" << endl;
        }
        else if (checkL(flags) && checkA(flags)){
            lsL(file.at(i));
        }
        else if (checkL(flags) && checkR(flags)){
            cout << "L and R" << endl;
        }
        else if (checkA(flags) && checkR(flags)){
            cout << "A and R" << endl;
        }
        else if(checkL(flags)){
            if(file.at(i).at(0) != '.')
                lsL(file.at(i));
        }
        else if (checkA(flags)){
            cout << file.at(i) << " ";
        }
        else if (checkR(flags)){
            cout << "haven't done recursion yet" << endl;
        }
        else{
            if(file.at(i).at(0) != '.'){
                cout << file.at(i) << " ";
            }
        }
}
    closedir(dirp);
}

bool noCaseComp(const string &one, const string &two){
    //change sort to ignore first char if file is dot.
    for(unsigned i=0; (i < one.size()) && (i<two.size());++i){
        if(tolower(one.at(i)) < tolower(two.at(i)))
            return true;
        else if (tolower(one.at(i))>tolower(two.at(i)))
            return false;
    }
    return (one.size() < two.size());
}
