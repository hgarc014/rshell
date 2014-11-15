#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <pwd.h>

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

    if(pass != NULL && gethostname(machine,SZ) != -1){
        user = pass->pw_name;
        all = user + "@" + machine + "$ ";
    }else{
        if(pass == NULL)
            perror("hostName");
        else
            perror("MachineName");
        cout << LINE << endl
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
        }
        else if (input.find("<") != string::npos)
        {
            cout << "found < only" << endl;
        }
        else if (input.find(">>") != string::npos)
        {
            cout << "found >> only" << endl;
        }
        else if (input.find(">") != string::npos)
        {
            cout << "found > only" << endl;
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

