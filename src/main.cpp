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

int main()
{
    cout << "Welcome to Rshell" << endl;
    string input;
    getline(cin, input);
//    cin >> input;

    int pid = fork();
    if(pid == 0){
        char *argv[10];
        argv[0] = new char[10];
        strcpy(argv[0], input.c_str());
        cout << "input: " << argv[0] << endl;
        argv[1]="-a";
        argv[2]=0;
       if(execvp(argv[0], argv) == -1){
            perror("execvp");
            exit(1);
        }
    }else{
        wait(NULL);
    }

    return 0;
}

