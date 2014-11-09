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
#include <boost/format.hpp>
#include <algorithm>

#include <iostream>
#include <iomanip>
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

const int MAX_LNK = 3;
const int MAX_USR = 10;
const int MAX_SZ = 10;


void lsL(const string path, const string &name);
void checkinput(const int, const string &s);
bool noCaseComp(const string &one, const string &two);
void lsR(vector<string> &file, string path, bool isA, bool isL);

int main(int argc, char*argv[])
{
    int flags = 0;
    vector<string> input;

    for(int i =1; i < argc; ++i)
    {
        if(argv[i][0] == '-')
        {
            for(int j = 1; argv[i][j] != 0 && argv[i][j] != ' ';j++)
            {
                if(argv[i][j] == 'a')
                    flags |= FLAG_a;
                else if(argv[i][j] == 'l')
                    flags |= FLAG_l;
                else if(argv[i][j] == 'R')
                    flags |= FLAG_R;
            }
        }
        else
        {
            input.push_back(argv[i]);
        }
    }

    string s = ".";
    if(input.size() > 0){
        sort(input.begin(), input.end(), noCaseComp);
        for(unsigned i=0; i < input.size(); ++i)
        {
            s = input.at(i);
            if(input.size() > 1 && !(checkR(flags)))
                cout << s << ":" << endl;
            checkinput(flags,s);
            cout << endl;
        }
    }
    else
    {
        checkinput(flags,s);
        cout << endl;
    }
    return 0;
}




////////////////////////////////////////////////////////////////
void lsL(const string path, const string &name)
{

    struct stat statbuf;
    if(stat(path.c_str(), &statbuf) == -1){
        perror("stat");
        exit(1);
    }

    //cout << "total: " << statbuf.st_blksize;

    if(S_ISDIR(statbuf.st_mode))
        cout << "d";
    else if(S_ISCHR(statbuf.st_mode))
        cout << "c";
    else if(S_ISBLK(statbuf.st_mode))
        cout << "b";
    else if(S_ISFIFO(statbuf.st_mode))
        cout << "p";
    else if(S_ISLNK(statbuf.st_mode))
        cout << "l";
    else if(S_ISSOCK(statbuf.st_mode))
        cout << "s";
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
    eldash; if(statbuf.st_mode & S_IROTH)
        cout << "r";
    eldash;
    if(statbuf.st_mode & S_IWOTH)
        cout << "w";
    eldash;
    if(statbuf.st_mode & S_IXOTH)
        cout << "x";
    eldash;
    string time = ctime(&statbuf.st_ctime);
    time = time.substr(3, time.size()-12) + " ";
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
    cout << " " << left << setw(MAX_LNK) << statbuf.st_nlink << ' ' << setw(MAX_USR) 
        << usr->pw_name << setw(MAX_USR)  << gp->gr_name << setw(MAX_SZ)
        << statbuf.st_size << setw(MAX_USR + 5) << time << /*setw(5) <<*/ name;

    cout << endl;
}

////////////////////////////////////////////////////////////////
void lsR(vector<string> &file, string path, bool isA, bool isL)
{
    cout << endl << path << ":" << endl;
    vector<string > dirs;
    for(unsigned i=0; i < file.size(); ++i)
    {
        struct stat statbuf;
        if(isL)
        {
            string temp = path + "/" + file.at(i);
            if(file.at(i).at(0) != '.')
                lsL(temp,file.at(i));
            else if (isA)
                lsL(temp,file.at(i));
        }
        else{
            if(file.at(i).at(0) != '.')
                cout << file.at(i) << ' ';
            else if(isA)
                cout << file.at(i) << ' ';
        }
        string temp = path + "/" + file.at(i);
        //        cout << endl << "tpath=" << temp << endl;
        if(stat(/*file.at(i).*/temp.c_str(), &statbuf) == -1){
            perror("stat");
            exit(1);
        }
        if(S_ISDIR(statbuf.st_mode))
        {
            if(file.at(i).at(0) != '.')
            {
                dirs.push_back(file.at(i));
            }
            else if(isA)
                dirs.push_back(file.at(i));
        }
    }
    unsigned i =0;
    if(isA)
        i = 2;
    for(; i < dirs.size(); ++i)
    {
        file.clear();
        if(dirs.at(i) == "..")
            continue;
        //        cout << endl << "DIR=" << dirs.at(i) << endl;
        string temp = path + "/" + dirs.at(i);
        DIR *dirp = opendir(temp.c_str() /*dirs.at(i).c_str()*/);
        dirent *direntp;
        if(dirp == NULL)
        {
            perror("opendir");
            exit(1);
        }
        while((direntp = readdir(dirp)))
        {
            if(direntp == NULL)
            {
                perror("readdir");
                exit(1);
            }
            string s = direntp->d_name;
            file.push_back(s);
        }
        sort(file.begin(),file.end(),noCaseComp);
        cout << endl;
        lsR(file, path + "/" + dirs.at(i), isA, isL);
    }
}

////////////////////////////////////////////////////////////////
void checkinput(const int flags, const string &s)
{
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
            lsR(file, s, true, true);
            break;
        }
        else if (checkL(flags) && checkA(flags)){
            lsL(s + "/" + file.at(i),file.at(i));
        }
        else if (checkL(flags) && checkR(flags)){
            lsR(file, s, false, true);
            break;
        }
        else if (checkA(flags) && checkR(flags)){
            lsR(file, s, true, false);
            break;
        }
        else if(checkL(flags)){
            if(file.at(i).at(0) != '.')
                lsL(s + "/" + file.at(i),file.at(i));
        }
        else if (checkA(flags)){
            cout << /*left << setw(5) <<*/ file.at(i) << " ";
        }
        else if (checkR(flags)){
            lsR(file, s, false, false);
            break;
        }
        else{
            if(file.at(i).at(0) != '.'){
                cout << /*left << setw(5) << */file.at(i) << " ";
            }
        }
    }
    cout << endl;
    closedir(dirp);
}

////////////////////////////////////////////////////////////////
bool noCaseComp(const string &one, const string &two)
{
    unsigned i = 0,
             j = 0;
    if(one.size() > 1 && one.at(0) == '.')
        ++i;
    if(two.size() > 1 && two.at(0) == '.')
        ++j;

    for(; (i < one.size()) && (j < two.size()); ++i,++j){
        if(tolower(one.at(i)) < tolower(two.at(j)))
            return true;
        else if (tolower(one.at(i))>tolower(two.at(j)))
            return false;
    }
    return (one.size() < two.size());
}
