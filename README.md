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


#------------REMOVE BELOW----------------

git tag --a NAME -m "MESSAGE"
git tag
git push --tags

##Commands to be tested...

#shows and works correctly with an error at the end
ls -a  && ls && l
#shows AND will stop and not echo
ls && pwd  && x && echo cant see me

#shows OR will continue until a correct
l || x || j || echo i should be printed
#shows OR will stop after a correct command
ls || x || echo i wont be printed

#shows commands execute with ; at the end
ls -al; ls -a; pwd;
#shows commands execute with errors in a ; line example
ls; x ; echo should see error for x and see me;
#shows commands still execute if no ; is at the end
ls; ls -l; pwd; echo test with no semi colon at end

#shows command works without a connector
pwd
#shows command works with spaces between command and arguements with connector
    ls            -a         -l    ;
#shows command works with spaces between command and arguements without connector
ls        -al

#
clear; echo this is a #test
#
ls -al #; ls; pwd

#
echo spacing               is              large  not with echo




