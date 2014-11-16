#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <fcntl.h>

#include <boost/tokenizer.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;
using namespace boost;

#define TOKEN tokenizer<char_separator<char> >
#define SEP char_separator<char>

void executeCommand(const string &input, const char cmd[]);
void createCommand(const string &input,const char cmd[]);
void inputc(const string &input, bool is3);
void output(const string &input, bool isAp);
void piping(const string &input);

const char SEMIS[] =";";
const char ANDS[] = "&&";
const char ORS[] = "||";
const char EXIT[] = "exit";
const int SZ = 50;
const string LINE(40, '-');

int main(){

    char machine[SZ];
    string user,
           all,
           input;

    struct passwd *pass = getpwuid(getuid());
    int host = gethostname(machine,SZ);

    if(pass != NULL && host != -1){
        user = pass->pw_name;
        all = user + "@" + machine + "$ ";
    }else{
        if(pass == NULL)
            perror("hostName");
        if(host == -1)
            perror("MachineName");
        cout << endl << LINE << endl
            << "Couldn't find userLogin or HostMachine displaying $ instead" << endl;
        all = "$ ";
    }

    cout << all;
    while(getline(cin, input)){
        if(input.find("#") != string::npos){
            input = input.substr(0,input.find("#"));
        }
        if(input.find("&&") != string::npos){
            createCommand(input, ANDS);
        }else if(input.find("||") != string::npos){
            createCommand(input, ORS);
        }
        else if(input.find("|") != string::npos)
        {
            cout << "Do something for piping" << endl;
            piping(input);
        }
        else if (input.find("<") != string::npos)
        {
            cout << "found < only" << endl;
            inputc(input,false);
        }
        else if (input.find(">>") != string::npos)
        {
            cout << "found >> only" << endl;
            output(input,true);
        }
        else if (input.find(">") != string::npos)
        {
            cout << "found > only" << endl;
            output(input,false);
        }
        else{
            createCommand(input, SEMIS);
        }
        cout << all;
    }
}

void createCommand(const string &input,const char cmd[]){

    SEP sep(cmd);
    TOKEN tok(input, sep);
    TOKEN::iterator it = tok.begin();
    for(; it != tok.end(); ++it){
        int id =fork();
        if(id == -1){
            perror("FORK");
            exit(1);
        }else if(id == 0){
            string temp = *it;
            executeCommand(temp,cmd);
        }else{
            int status = 0;
            if( wait(&status)== -1){
                perror("WAIT");
                exit(1);
            }
            bool childDieNormal = WIFEXITED(status);
            int childExit = WEXITSTATUS(status);
            if(childDieNormal && childExit == 1 && strcmp(cmd, ANDS) == 0){
                return;
            }else if(childDieNormal && childExit == 0 && strcmp(cmd, ORS) == 0){
                return;
            }else if(childDieNormal && childExit == 10){
                exit(0);
            }
        }
    }
}

void executeCommand(const string &input,const char cmd[]){
    char *argv[SZ];

    SEP sep(" ");
    TOKEN tok(input, sep);
    TOKEN::iterator it = tok.begin();
    if(it != tok.end() && (*it) == EXIT){
        exit(10);
    }
    int i = 0;
    for(; it != tok.end(); ++it,++i){
        argv[i] =new char[(*it).size()];
        strcpy(argv[i],(*it).c_str());
    }
    argv[i] = 0;
    if(execvp(argv[0],argv) == -1){
        perror(argv[0]);
        for(it = tok.begin(); it != tok.end(); ++it)
            delete [] argv[i];
        exit(1);
    }
    for(it = tok.begin(); it != tok.end(); ++it)
        delete [] argv[i];
}

void inputc(const string &input, bool is3)
{
    if(is3)
        ;
    char *argv[SZ];
    int fd = open("file",O_RDONLY);
    //int oldstd = dup(0);
    if(close(0) == -1)
    {
        perror("close");
        exit(1);
    }
    if(dup(fd)==-1)
    {
        perror("dup");
        exit(1);
    }
    argv[0] = new char[40];
    argv[1] = NULL;
    strcpy(argv[0],"cat");
    execvp(argv[0],argv);
    perror("execvp");
}

void output(const string &input, bool isAp)
{
    string val;
    char *argv[SZ];
    int redirect = 1;
    int flags = O_RDWR|O_CREAT;
    if(input.find("2>") != string::npos)
        redirect = 2;
    if(isAp)
    {
        val = ">>";
        flags |= O_APPEND;
    }
    else
    {
        val = ">";
        flags |= O_TRUNC;
    }
    string l = input.substr(0,input.find(val));
    string r = input.substr(input.find(val)+val.size());
    //    cout << "l=" << l << endl
    //        << "r=" << r << endl;
    SEP sep(" ");
    TOKEN tok(l,sep);
    TOKEN tok2(r,sep);
    TOKEN::iterator it = tok.begin();
    TOKEN::iterator it2 = tok2.begin();
    int i;
    for(i =0;it != tok.end(); ++it,++i)
    {
        argv[i] = new char[(*it).size()];
        strcpy(argv[i],(*it).c_str());
    }
    argv[i]=NULL;

    if(it2 == tok2.end())
    {
        cout << "ERROR NO FILE" << endl;
        return;
    }
    int fd = open((*it2).c_str(),flags,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if(fd ==-1)
    {
        perror("open");
        exit(1);
    }
    if(argv[0] == NULL)
    {
        cout << "no command" << endl;
        close(fd);
        return;
    }
    int oldstd = dup(redirect);
    if(oldstd == -1)
    {
        perror("dup");
        exit(1);
    }
    if(close(redirect)==-1)
    {
        perror("close");
        exit(1);
    }
    if(dup(fd)==-1)
    {
        perror("dup");
        exit(1);
    }
    //    cout << "should be deleted!!!" << endl;
    //    char *argv[10];
    //    argv[0]=new char[40];
    //    strcpy(argv[0],"l");
    //    argv[1]=NULL;
    execvp(argv[0],argv);
    perror("execvp");
    exit(1);
    oldstd = oldstd;
}

void piping(const string &input)
{
    int fd[2];
    if(pipe(fd)==-1)
    {
        perror("pipe");
        exit(1);
    }
    int pid=fork();
    if(pid == -1)
    {
        perror("FORK");
        exit(1);
    }
    else if (pid == 0)
    {
        char *argv[SZ];
        argv[0]= new char[40];
        strcpy(argv[0],"ls");
        cout << "executing ls" << endl;
        argv[1]= NULL;
        if(close(1)==-1)
        {
            perror("close");
            exit(1);
        }
        if(dup(fd[1])==-1)
        {
            perror("dup");
        }
        execvp(argv[0],argv);
        perror("execvp");
    }
    else
    {
        if(wait(NULL)==-1)
        {
            perror("WAIT");
            exit(1);
        }
        if(close(fd[1])==-1)
        {
            perror("close");
            exit(1);
        }
        if(close(0)==-1)
        {
            perror("close");
            exit(1);
        }
        if(dup(fd[0])==-1)
        {
            perror("dup");
        }
        char *argv[SZ];
        argv[0] = new char[40];
        argv[1] = new char[40];
        strcpy(argv[0],"grep");
        strcpy(argv[1],".rem");
        argv[2]=NULL;
        execvp(argv[0],argv);
        perror("execvp");
    }
}

