#rshell

Runs commands with execvp in order to produce a terminal experience. allows use with commands that are found with the execvp function.


##License information in LICENSE file
https://github.com/hgarc014/rshell/blob/master/LICENSE

##Run program/install

once the directory has been copied install the program by going into the rshell directory

`cd rshell`

then install the files by executing

`make`

then run the program by running

`bin/rshell`


##Bugs rshell

1. having different symbols in one line will cause an error in execvp
  * `ls && ls || ls ;exit`
  * running | or & will cause errors because we do not have piping or giving back terminal implemented
2. Having same symbol without arguements will not throw an error
  * `ls && && && ls` will execute ls twice and ignore `&&` symbols (same for `||` and `;`)
  * this is also the cse if && is first
    * `&& ls`
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

##Bugs ls

1. hidden files are colored farther due to setw

