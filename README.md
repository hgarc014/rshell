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
  * Acts similarly when given `&&&` `|||` `;;;`
3. Having only one symbol will not execute commands
4. will not ask for continued input if an input is like `ls &&` will just execute
command and then look for a new line of commands
5. if a bash script contains the let command rshell will not know what this is.
6. if hostMachine is larger than 50 characters it will be shortened
  * if hostMachine or HostName could not be determined an error message will print
  and the prompt will be a `$` instead of hostname and username
7. cannot run background programs
  * `google-chrome &`
8. cannot run a command in quotes
  * `"ls"`


#------------REMOVE BELOW----------------

git tag --a NAME -m "MESSAGE"
git tag
git push --tags

