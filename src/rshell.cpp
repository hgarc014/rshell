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
}

void executeRedirect(const string &input,int nin, int nout, int nerr)
{
    char *argv[SZ];
    SEP sep(" ");
    TOKEN tok(input,sep);
    TOKEN::iterator it = tok.begin();
    if(it == tok.end())
    {
        cout << "empty command" << endl;
        exit(1);
    }
    int i = 0;
    for(;it != tok.end(); ++it,++i)
    {
        argv[i] = new char[(*it).size()];
        strcpy(argv[i],(*it).c_str());
    }
    argv[i] = NULL;
    if(nin >= 0)
    {
        close(0);
        dup(nin);
    }
    if(nout >= 0)
    {
        close(1);
        dup(nout);
    }
    if(nerr >= 0)
    {
        close(2);
        dup(nerr);
    }
    execvp(argv[0],argv);
    perror("execvp");
    for(it = tok.begin(); it != tok.end(); ++it)
        delete [] argv[i];
    exit(1);
}
int openFile(const string &file, int flags)
{
    SEP sep(" ");
    TOKEN tok(file,sep);
    TOKEN::iterator it = tok.begin();
    if(it == tok.end())
    {
        cout << "no file" << endl;
        exit(1);
    }
    int fd = open((*it).c_str(),flags,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    if(fd == -1)
    {
        perror("open");
    }
    return fd;
}

void inputc(const string &input, bool is3)
{
    //    int fd = open("r.rem",O_RDONLY);
    //    close(0);
    //    dup(fd);
    //        char *test[SZ];
    //        test[0] = new char[40];
    //        strcpy(test[0],"wc");
    //        test[1] = NULL;
    //        execvp(test[0],test);
    //        perror("execvp");
    //        exit(1);
    //
    //        close(fd);
    string v = "<";
    if(is3)
    {
        v = "<<<";
        cout << "only take input from string";
    }
//    char *argv[SZ];
    string l = input.substr(0,input.find(v));
    string r = input.substr(input.find(v) + v.size());
    string old = r;
    int outp = -1;
    int inp = -1;
    if(!is3)
    {
        inp = openFile(old,O_RDONLY);
    }
    if(r.find(">") != string::npos)
    {
        cout << "found > " << endl;
        old = old.substr(0,r.find(">"));
        int flags = O_RDWR|O_CREAT;
        int found =-1; 
        if(r.find(">>") != string::npos)
        {
            flags |= O_APPEND;
            found = r.find(">>") + 2;
        }
        else
        {
            found = r.find(">")+1;
            flags |= O_TRUNC;
        }
        r = r.substr(found);
        outp = openFile(r,flags);
    }

//    string outp = " ";

//    if(r.find(">") != string::npos)
//    {
//        int flags = O_RDWR|O_CREAT;
//        unsigned found = r.find(">>");
//        if(found != string::npos)
//        {
//            flags |= O_APPEND;
//        }
//        else// if(found == string::npos)
//        {
//            found = r.find(">");
//            flags |= O_TRUNC;
//        }
//        r = r.substr(found);
//        string o = r.substr(r.find(">"));
//        outp = openFile(o,flags);
//    }
        
//        cout << "should close std out and replace by file" << endl;
//        outp = r.substr(r.find(""));
//        r = r.substr(0,r.find(">"));
//        //output(r,r.find(">>"));
//        //exit(1);
//    }
//    SEP sep(" ");
//    TOKEN tok(l,sep);
//    TOKEN tok2(r,sep);
//    TOKEN tok3(outp,sep);
//    TOKEN::iterator it=tok.begin();
//    TOKEN::iterator it2 = tok2.begin();
//    TOKEN::iterator it3 = tok3.begin();
    int pid = fork();
    if(pid == -1)
    {
        perror("fork");
    }
    else if (pid == 0)
    {
       cout << "l=" << l << endl
            << "r=" << r << endl
            << "old=" << old << endl;
        executeRedirect(l,inp,outp,-1);
        //        if(it2 == tok2.end())
        //        {
        //            cout << "ERROR NO FILE" << endl;
        //            exit(1);
        //        }
        //        int i = 0;
        //        for(;it != tok.end();++it,++i)
        //        {
        //            argv[i] = new char[(*it).size()];
        //            strcpy(argv[i],(*it).c_str());
        //            cout << i << "=" << argv[i] << endl;
        //        }
        //        argv[i] = NULL;
        //
        //
        //        int fd = open((*it2).c_str(),O_RDONLY,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
        //        if(fd == -1)
        //        {
        //            perror("open");
        //            exit(1);
        //        }
        //        //int oldstd = dup(0);
        //        if(close(0) == -1)
        //        {
        //            perror("close");
        //            exit(1);
        //        }
        //        if(dup(fd)==-1)
        //        {
        //            perror("dup");
        //            exit(1);
        //        }
        //        cout << "bout to execute" << endl;
        //        //        char *test[SZ];
        //        //        test[0] = new char[40];
        //        //        strcpy(argv[0],"wc");
        //        //        test[1] = NULL;
        //
        //        execvp(argv[0],argv);
        //        perror("execvp");
        //        exit(1);
    }
    else
    {
        wait(NULL);
        if(inp != -1)
        {
            close(inp);
        }
        if(outp != -1)
        {
            close(outp);
        }
    }
}

void output(const string &input, bool isAp)
{
    string val;
    //char *argv[SZ];
    //    int redirect = 1;
    int flags = O_RDWR|O_CREAT;
    //    if(input.find("2>") != string::npos)
    //        redirect = 2;
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
    int found = input.find(val);
    if(input.find("2>") !=string::npos)
        --found;
    string l = input.substr(0,found);
    string r = input.substr(input.find(val)+val.size());
    cout << "l=" << l << endl;
    cout << "r=" << r << endl;
        int fd = openFile(r,flags);
        if(fd == -1)
        {
            perror("open");
            return;
        }
    //    SEP sep(" ");
    //    TOKEN tok(l,sep);
    //    TOKEN tok2(r,sep);
    //    TOKEN::iterator it = tok.begin();
    //    TOKEN::iterator it2 = tok2.begin();
    int pid=fork();
    if(pid ==-1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        //        if(it2 == tok2.end())
        //        {
        //            cout << "ERROR NO FILE" << endl;
        //            exit(1);
        //        }
        if(input.find("2>") != string::npos)
            executeRedirect(l,-1,-1,fd);
        else
            executeRedirect(l,-1,fd,-1);
        //        int i;
        //        for(i =0;it != tok.end(); ++it,++i)
        //        {
        //            argv[i] = new char[(*it).size()];
        //            strcpy(argv[i],(*it).c_str());
        //        }
        //        argv[i]=NULL;
        //
        //        int fd = open((*it2).c_str(),flags,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
        //        if(fd ==-1)
        //        {
        //            perror("open");
        //            exit(1);
        //        }
        //        if(argv[0] == NULL)
        //        {
        //            cout << "no command" << endl;
        //            close(fd);
        //            exit(1);
        //        }
        //
        //        if(dup(redirect) == -1)
        //        {
        //            perror("dup");
        //            exit(1);
        //        }
        //        if(close(redirect)==-1)
        //        {
        //            perror("close");
        //            exit(1);
        //        }
        //        if(dup(fd)==-1)
        //        {
        //            perror("dup");
        //            exit(1);
        //        }
        //        execvp(argv[0],argv);
        //        perror("execvp");
        //        exit(1);
    }
    else
    {
        wait(NULL);
        close(fd);
    }
}

void piping(const string &input)
{
    int fd[2];
    string l = input.substr(0,input.find("|"));
    string r = input.substr(input.find("|")+1);
    SEP sep(" ");
    TOKEN tok(l,sep);
    TOKEN tok2(r,sep);
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
        //if next iteration is existant
        //then call pipe on shorter version
        //or a loop depending on which is easier
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

