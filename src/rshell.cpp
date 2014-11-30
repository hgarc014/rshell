#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pwd.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>

#include <boost/tokenizer.hpp>

#include <cstdlib>
#include <iostream>
#include <string>
#include <map>

using namespace std;
using namespace boost;

#define TOKEN tokenizer<char_separator<char> >
#define SEP char_separator<char>
//checksyscalls does not find this
//#define CHECKNEG(x,y) if(x==-1){perror(y);exit(1);} 

//constant variables
const char SEMIS[] =";";
const char ANDS[] = "&&";
const char ORS[] = "||";
const char EXIT[] = "exit";
const char CD[] = "cd";
const int SZ = 50;
const string LINE(40, '-');

//functions
void executeCommand(const string &input, const char cmd[]);
void createCommand(const string &input,const char cmd[]);
void inputc(const string &input, bool is3);
void output(const string &input, bool isAp,int inp=-1);
void piping(const string &input);
string getcmd(string file);
void replacetil(string &s);

//signal handlers
void handle_c(int signum);
void handle_z(int signum);

int main(){
//    signal(SIGINT,handle_c);
    signal(SIGTSTP,handle_z);

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

    char curdir[BUFSIZ];
    if(getcwd(curdir,BUFSIZ)==NULL)
    {
        perror("getcwd");
        exit(1);
    }

    cout << curdir << endl << all;
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
        if(getcwd(curdir,BUFSIZ)==NULL)
        {
            perror("getcwd");
            exit(1);
        }
        cout << curdir << endl << all;
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
    //    if(it != tok.end() && (*it) == EXIT){
    //        exit(10);
    //    }
    if(it != tok.end())
    {
        if((*it) == EXIT)
            exit(10);
        if((*it) == CD)
        {
            ++it;
            if(it == tok.end())
            {
                string home = "~";
                replacetil(home);
                if(chdir(home.c_str())==-1)
                {
                    perror("chdir");
                    exit(1);
                }
            }
            else
            {
                if(chdir((*it).c_str())== -1)
                {
                    perror("chdir");
                    exit(1);
                }
            }
            return;
        }
        //call function
    }
    int i = 0;
    for(; it != tok.end(); ++it,++i){
        argv[i] =new char[(*it).size()];
        strcpy(argv[i],(*it).c_str());
    }
    argv[i] = 0;

    string first = getcmd(argv[0]);
    //cout << "first=" << first << endl << endl;

    if(execv(first.c_str(),argv) == -1){
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

    string path = getcmd(argv[0]);
    execv(path.c_str(),argv);
    perror("execv");
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
        return -1;
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
            if(write(fd2[1],buf,strlen(buf))==-1)
            {
                perror("write");
                exit(1);
            }
            if(close(fd2[1])==-1)
            {
                perror("close");
            }
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
        if(inp == -1)
        {
            return;
        }
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
        if(inp == -1)
        {
            return;
        }
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
    if(fd == -1)
        return;
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
                if(newin == -1)
                    exit(1);
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
                if(write(fd2[1],buf,strlen(buf))==-1)
                {
                    perror("write");
                    exit(1);
                }
                if(close(fd2[1])==-1)
                {
                    perror("close");
                    exit(1);
                }
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

string getcmd(string file)
{
    char *path = getenv("PATH");
    if(path == NULL)
    {
        perror("getenv");
        exit(1);
    }
    string temp = path;
    SEP sep(":");
    TOKEN tok(temp,sep);
    TOKEN::iterator it = tok.begin();
    DIR *dirp;
    dirent *direntp;
    for(;it != tok.end(); ++it)
    {
        string temp = *it;
        replacetil(temp);
        dirp = opendir(temp.c_str());
        if(dirp == NULL)
        {
            perror("opendir");
            exit(1);
        }
        while((direntp=readdir(dirp)))
        {
            if(direntp == NULL)
            {
                perror("readdir");
                exit(1);
            }
            if(file == direntp->d_name)
            {
                //cout << "found " << file << " at=" << temp << endl;
                return temp + "/" + file;
            }
        }
    }
    return file;
}

void replacetil(string &s)
{
    char *h = getenv("HOME");
    if(h == NULL)
    {
        perror("getenv");
        exit(1);
    }
    string home = h;
    home +="/";

    if(s.find("~/") == 0)
    {
        s.replace(0,2,home,0,home.size());
    }
    else if(s.find("~") == 0)
    {
        s.replace(0,1,home,0,home.size());
    }
}

void handle_c(int signum)
{
    cout << "ctrl + c was pressed" << endl;

}
void handle_z(int signum)
{
    cout << "ctrl + z was pressed" << endl;
    raise(SIGSTOP);
}
