#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

//void executeCommand(string input){
//    if(input.find(" ") != string::npos){
//        string cmd = input.substr(0, input.find(" "));
//        cmd = cmd.substr(0, cmd.find(";"));
//        const *argv[]
//        if(execvp() == -1){
//            perror("execvp");
//            exit(1);
//        }
//    }
//}

void executeCommand(string input){
    //int sz = input.size()/2;
    cerr << "input=" << input << endl;
    char *argv[50];
    int space = -1;
    int i = 0;
    if(input.find(" ") == string::npos){

    }
    for( ; input.find(" ") != string::npos; ++i){
        space = input.find(" ");
        argv[i] = new char[input.substr(0, space).size()];
        strcpy(argv[i],input.substr(0, space).c_str());
        cerr << "Added=" << input.substr(0,space) << endl;
        input = input.substr(space+1);
        cerr << "next=" << input << endl;
    }
    argv[i] = new char[input.size()];
    strcpy(argv[i], input.c_str());
    argv[i + 1] = 0;
    if(execvp(argv[0],argv) == -1){
        perror("execvp in executeCommand");
        exit(1);
    }
}

int main()
{
    cout << "$";
    string input;
//
    while(getline(cin, input)){
        //TODO: do other commands before exiting
        if(input.find("exit") != string::npos)
            exit(0);
        cout << "input=" << input << endl;
        executeCommand(input.substr(0,input.find(";")));
        cout << "$";
    }

//    if(input == "exit"){
//        exit(1);
//    }
//    if(input.find("#") != string::npos)
//        input = input.substr(0, input.find("#"));
//    int pid = fork();
//    if(pid == 0){
//        char *argv[10];
//        argv[0] = new char[10];
//        strcpy(argv[0], input.c_str());
//        cout << "input=" << argv[0] << endl;
//        argv[1]=0;
//        argv[2]=0;
//       if(execvp(argv[0], argv) == -1){
//            perror("execvp");
//            exit(1);
//        }
//    }else{
//        if( wait(0))
//            perror("Wait");
//    }

    return 0;
}

