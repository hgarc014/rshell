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




#REMOVE BELOW

git tag --a NAME -m "MESSAGE"
git tag
git push --tags


* can input a series of `&&`, `||`, or `;` in one line.
  * having different symbols for one line will cause execvp to fail
    * Ex. `ls && ls || ls ; ls`
  * Having the same symbol without arguements will be ignored by the rshell
    * Ex. `ls && && && ls`
    * this will lead to ls being printed twice ignoring the other &&s
* rshell will not ask for continued input like Bash
  * ex. `ls &&`
  * this will execute ls and not look for further input
* rshell ignores everything after the # (comment) symbol.
  * Bash will give an error on a command like `ls#` However, rshell will execute ls
  * rshell will also terminate anything after # even if in quotes
* rshell can execute commands found using execvp
* rshell displays username and hostMachine like other terminals
  * rshell is limited to 50 characters for hostMachine name
  * rshell will print out a error message and regular $ if hostName or Machine could not be found
* rshell can only handle at least 100,000 flags.
  * so, anything more than that may have the command fail.
* rshell executes commands before exiting terminal
  * Ex. `ls;ls;ls;pwd;exit`
* rshell cannot use let command in a bash script
*
