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
void output(const string &input, bool isAp,int inp=-1);
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
            piping(input);
        }
        else if (input.find("<<<") != string::npos)
        {
            inputc(input,true);
        }
        else if (input.find("<") != string::npos)
        {
            inputc(input,false);
        }
        else if (input.find(">>") != string::npos)
        {
            output(input,true);
        }
        else if (input.find(">") != string::npos)
        {
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
        for(i = 0,it = tok.begin(); it != tok.end(); ++it,++i)
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
        if(close(0)==-1)
        {
            perror("close");
            exit(1);
        }
        if(dup(nin)==-1)
        {
            perror("dup");
            exit(1);
        }
    }
    if(nout >= 0)
    {
        if(close(1)==-1)
        {
            perror("dup");
            exit(1);
        }
        if(dup(nout)==-1)
        {
            perror("dup");
            exit(1);
        }
    }
    if(nerr >= 0)
    {
        if(close(2)==-1)
        {
            perror("dup");
            exit(1);
        }
        if(dup(nerr)==-1)
        {
            perror("dup");
            exit(1);
        }
    }
    execvp(argv[0],argv);
    perror("execvp");
    for(i = 0,it = tok.begin(); it != tok.end(); ++it,++i)
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
        exit(1);
    }
    return fd;
}

void inputc(const string &input, bool is3)
{
    string v = "<";
    string more;
    int outp = -1;
    int inp = -1;
    int fd2[2];
    if(is3)
    {
        v = "<<<";
        if(input.find("\"") != string::npos)
        {
            more = input.substr(input.find("\"")+1);
            if(more.find("\"") == string::npos)
            {
                cout << "invalid use" << endl;
                return;
            }
            more = more.substr(0,more.find("\""));
            if(pipe(fd2) ==-1)
            {
                perror("pipe");
                exit(1);
            }
            char *buf = new char[more.size()];
            strcpy(buf,more.c_str());
            write(fd2[1],buf,strlen(buf));
            close(fd2[1]);
            inp = fd2[0];
            delete [] buf;
        }
        else
        {
            cout << "invalid use" << endl;
            return;
        }
    }
    //    char *argv[SZ];
    string l = input.substr(0,input.find(v));
    string r = input.substr(input.find(v) + v.size());
    string old = r;
    if(!is3)
    {
        inp = openFile(old,O_RDONLY);
    }
    if(r.find(">") != string::npos)
    {
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
    int pid = fork();
    if(pid == -1)
    {
        perror("fork");
    }
    else if (pid == 0)
    {
        executeRedirect(l,inp,outp,-1);
    }
    else
    {
        if(wait(NULL)==-1)
        {
            perror("wait");
            exit(1);
        }

        if(inp != -1)
        {
            if(close(inp)==-1)
            {
                perror("close");
                exit(1);
            }
        }
        if(outp != -1)
        {
            if(close(outp)==-1)
            {
                perror("close");
                exit(1);
            }
        }
    }
}

void output(const string &input, bool isAp, int inp)
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
    if(input.find("1>") !=string::npos)
        --found;
    string l = input.substr(0,found);
    string r = input.substr(input.find(val)+val.size());
    int fd = openFile(r,flags);
    int pid=fork();
    if(pid ==-1)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        if(input.find("2>") != string::npos)
            executeRedirect(l,inp,-1,fd);
        else
            executeRedirect(l,inp,fd,-1);
    }
    else
    {
        if(wait(NULL)==-1)
        {
            perror("WAIT");
            exit(1);
        }
        if(close(fd)==-1)
        {
            perror("CLOSE");
            exit(1);
        }
    }
}

void piping(const string &input)
{
    int fd[2];
    int fd2[2];
    string l = input.substr(0,input.find("|"));
    string r = input.substr(input.find("|")+1);
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
        int newin = -1;
        if(l.find("<") != string::npos)
        {
            string more;
            if(l.find("<<<") == string::npos)
            {
                string file = l.substr(l.find("<")+1);
                l = l.substr(0,l.find("<"));
                newin = openFile(file,O_RDONLY);
            }
            else if(l.find("\"") != string::npos)
            {
                more = input.substr(input.find("\"")+1);
                if(more.find("\"") == string::npos)
                {
                    cout << "invalid use" << endl;
                    return;
                }
                more = more.substr(0,more.find("\""));
                if(pipe(fd2) ==-1)
                {
                    perror("pipe");
                    exit(1);
                }
                char *buf = new char[more.size()];
                strcpy(buf,more.c_str());
                write(fd2[1],buf,strlen(buf));
                close(fd2[1]);
                newin = fd2[0];
                delete [] buf;
            }
            else
            {
                cout << "incorrect usage" << endl;
                exit(1);
            }
        }
        executeRedirect(l,newin,fd[1],-1);
    }
    else
    {
        int status;
        if(wait(&status)==-1)
        {
            perror("WAIT");
            exit(1);
        }
        bool childDieNormal = WIFEXITED(status);
        int childExit = WEXITSTATUS(status);
        if(close(fd[1])==-1)
        {
            perror("close");
            exit(1);
        }
        if(childDieNormal && childExit == 1)
        {
            cout << "child exited badly(bad command)" << endl;
            return;
        }

        int oldstd = dup(0);
        if(oldstd == -1)
        {
            perror("dup");
            exit(1);
        }

        if(r.find("|") != string::npos)
        {
            if(close(0)==-1)
            {
                perror("close");
                exit(1);
            }
            if(dup(fd[0])==-1)
            {
                perror("dup");
                exit(1);
            }
            piping(r);
        }
        else if(r.find(">") != string::npos)
        {
            bool append = r.find(">>") != string::npos;
            output(r,append,fd[0]);
            return;
        }
        else
        {
            int apid = fork();
            if(apid == -1)
            {
                perror("FORK");
                exit(1);
            }
            if(apid == 0)
                executeRedirect(r,fd[0],-1,-1);
            else
            {
                if(wait(NULL) == -1)
                {
                    perror("WAIT");
                    exit(1);
                }
            }
        }
        if(close(0) == -1)
        {
            perror("CLOSE");
            exit(1);
        }
        if(close(fd[0])==-1)
        {
            perror("CLOSE");
            exit(1);
        }
        if(dup(oldstd)==-1)
        {
            perror("dup");
            exit(1);
        }
    }
}

