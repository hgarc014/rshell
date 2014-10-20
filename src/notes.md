* can input a series of &&, ||, or ; in one line.
  * having more than one of those symbols per line will break the program
* terminal will not ask for continued input
  * ex. ls &&# this will execute ls and not look for further input
* terminal ignores everything after the # symbol. Unlike other terminals it will find something like ls# while other terminals will give an error
* terminal can execute any command found in execvp
* terminal is limited to 50 characters for hostMachine name
* terminal displays username and hostMachine like other terminals
* terminal executes commands before exiting terminal
  * ex. ls;ls;ls;pwd;exit

* ssh hgarc014@hammmer.cs.ucr.edu
  *cs100
BUG:
    *
