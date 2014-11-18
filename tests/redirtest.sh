#!/bin/bash

# FILE MUST BE RUN FROM MAIN RSHELL FOLDER (where Makefile and LICENESE are)



#CMD=./bin/ls
TEST="# testing"
CHECK="# checking contents of:"
FILE1=newfile.rem
FILE2=newfile2.rem
FILE3=lower.rem
FILE4=upper.rem
ERROR1=error.rem

# $1 = test message
# $2 = command
function printcmd
{
    echo "$TEST $1 $2"
    eval $2
}
function checkcmd
{
    echo "$CHECK $1"
    eval "cat $1"
}

printcmd "redirect:" "echo this is a test > $FILE1"
checkcmd "$FILE1"

printcmd "append:" "echo test number 2 > $FILE1"
checkcmd "$FILE1"

printcmd "error redirect:" "cat doesntExist 2> $ERROR1"
checkcmd "$ERROR1"

printcmd "error append:" "cat cantDoIt 2>> $ERROR1"
checkcmd "$ERROR1"

#----------------------------------------------------------
printcmd "input:" "cat < $FILE1"
checkcmd "$FILE1"

#printcmd "" "" #input from STRING
#printcmd "" "" #input from STRING + redirect
#printcmd "" "" #input from STRING + piping

printcmd "input + redirect:" "cat < $FILE1 > $FILE2"
checkcmd "$FILE2"

printcmd "input + append:" "cat < $FILE1 >> $FILE2"
checkcmd "$FILE2"

#printcmd "mult in" ""
#printcmd "mult out" ""

#----------------------------------------------------------
printcmd "piping" "cat < $FILE1 | tr A-Z a-z | tee $FILE3 | tr a-z A-Z > $FILE4"


