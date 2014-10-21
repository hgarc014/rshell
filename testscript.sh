#!/bin/bash

#shows commands execute with ; at the end
ls -a; mkdir removeMe; touch removeMe/deleteMe;

#no end semi colon
ls   ;   ls removeMe/

#shows and works correctly with an error at the end and stopping
ls removeME/  && touch neat && l  && touch coolBean

#shows commands still work if close together
ls;pwd;

# shows and will execute all commands
cat neat && ls && rm neat

#shows OR will continue until a correct
l || x || j || echo i should be printed

#shows OR will stop after a correct command
ls || x || echo i wont be printed

#shows commands execute with errors in a ; line example
ls; x ; echo should see error for x and see me ;

#shows commands still execute if no ; is at the end
ls; ls -l; pwd; echo test with no semi colon at end

#shows command works with one command no connector
pwd
#shows command works with spaces between command and arguements with connector
    ls            -a         -l    ;    pwd    ;
#shows command works with spaces between command and arguements without connector
ls        -al

#shows comments are ignored in echo
clear; echo this is a #test ; vim removeMEagain

#shows comments are ignored in general
ls -al ; #ls; pwd

#shows
echo spacing               is              large  not with echo

#shows exit will work afterwards.
ls -a;

exit

