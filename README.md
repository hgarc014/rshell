#rshell


##License information in LICENSE file



##File List

* main.cpp
* Makefile



##Run program/install

once the directory has been copied install the program by going into the rshell directory

`cd rshell`

then install the files by executing

`make`

then run the program by running

`bin/rshell`


##Bugs

1. having different symbols in one line will cause an error in execvp
  * `ls && ls || ls ;exit`
2. Having same symbol without arguements will not throw an error
  * `ls && && && ls` will execute ls twice and ignore `&&` symbols (same for `||` and `;`)
  * Acts similarly when given `&&&`
3. will not ask for continued input if an input is like `ls &&` will just execute
command and then look for a new line of commands
4. if a bash script contains the let command rshell will not know what this is.
5. if hostMachine is larger than 50 characters it will be shortened
  * if hostMachine or HostName could not be determined an error message will print
  and the prompt will be a `$` instead of hostname and username


#------------REMOVE BELOW----------------

git tag --a NAME -m "MESSAGE"
git tag
git push --tags

##Commands to be tested...

ls && ls && l
ls && echo  && x && echo
ls && && && pwd
&& && pwd


l || x || j || ls
ls || x || ls
ls || || ||
|| || || ls
l || || || ls

ls; x ; echo;
ls; ls; pwd
ls -al; ls -a; pwd;
           ls            -a                -l           ;        ls;
ls        -al       ;     ls

clear; echo this is a #test
ls -al #; ls; pwd

echo spacing               is              large

ls &&& pwd
l ||| p ||| ls

