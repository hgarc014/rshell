#rshell

Runs commands with execvp in order to produce a terminal experience. allows use with commands that are found with the execvp function.


##License information 

Copyright (c) 2014, Henry Garcia
All rights reserved.

All files are licensed under the BSD3.

https://github.com/hgarc014/rshell/blob/master/LICENSE

##Run program/install

1. open a terminal and clone the repository
`git clone https://github.com/hgarc014/rshell.git`
2. cd into the project
`cd rshell/`
3. create executables
`make`
4. execute rshell
`bin/rshell`

##Bugs/Limitations rshell

1. having different symbols in one line will cause an error in execvp
  * `ls && ls || ls ;exit`
2. Having same symbol without arguements will not throw an error
  * `ls && && && ls` will execute ls twice and ignore `&&` symbols (same for `||` and `;`)
  * this is also the cse if && is first
    * `&& ls`
  * Acts similarly when given `&&&` `|||` `;;;`
3. Having only one symbol will not execute commands
4. will not ask for continued input if an input is like `ls &&` will just execute
command and then look for a new line of commands
5. if a bash script contains the let command rshell will not know what this is.
6. cannot run background programs
  * `google-chrome &`
7. cannot run a command in quotes
  * `"ls"`
8. running a command with piping and input or output in the center of the pipe line will result in a disconnection of the piping
9. input from string (<<<) results in no new line character.
10. having multiple outputs in a row will result in adding to all including the last one if input redirection was used first. However, if no input redirection was used first then an error will appear 
11. having multiple inputs will result in taking the first one being taken only
12. having multiple | without commands will result in an error being displayed
   * having multiple (<)s without commands will only result in grabing the first one
   * having multiple (>)s will result in no error but empting any files after the no command

##Bugs/Limitations ls

1. hidden files are colored farther due to setw
2. assumes a set terminal width (can be changed in source code)
3. There are extra newlines after ls output depending on type of flag/file parameters
4. files are sorted very similarly to ls, however shorter files are placed first on my version unlike ls. (ex re.file and readme.file)
5. files are also outputted alphabetically left to right unlike ls, which outputs alphabetically in columns (top down).

