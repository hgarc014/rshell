#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

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
void printUM();

const char SEMIS[] =";";
const char ANDS[] = "&&";
const char ORS[] = "||";
const char EXIT[] = "exit";
const int SZ = 50;

int main(){
    printUM();
    string input;
    while(getline(cin, input)){
        if(input.find("#") != string::npos){
            cout << input << endl;
            input = input.substr(0,input.find("#"));
            cout << input << endl;
        }
        if(input.find("&&") != string::npos){
            createCommand(input, ANDS);
        }else if(input.find("||") != string::npos){
            createCommand(input, ORS);
        }else{
            createCommand(input, SEMIS);
        }
        printUM();
    }
}
void printUM(){
    char *user = getlogin();
    char machine[SZ];

    if(user != NULL && gethostname(machine, SZ) != -1){
        cout << user << "@" << machine << "$ ";
    }else{
        cout << "$ ";
    }
//    delete [] user;
//    delete [] machine;
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
            }
            else if(id == 0){
                string temp = *it;
                executeCommand(temp,cmd);
            }
            else{
                int status = 0;
                if( wait(&status)== -1){
                    perror("wait");
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
    if((*it) == EXIT){
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
        exit(1);
    }
}

