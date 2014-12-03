#!/bin/bash

# FILE MUST BE RUN FROM MAIN RSHELL FOLDER (where Makefile and LICENESE are)
# files end in .rem for easy deletion

TEST="# testing"

DIR1="remove.rem"
FIL1="deleteme.rem"
FIL2="another.rem"
FIL3="something.rem"

# $1 = test message
# $2 = command
function printcmd
{
    echo -e "\n$TEST $1 $2"
    eval $2
}


printcmd "connector ; with ; at end:" "ls -a; mkdir $DIR1; touch $DIR1/$FIL1; ls remove;"
printcmd "connector ; without ; at end:" "ls $DIR1 ; ls "
printcmd "&& with error at end:" "ls $DIR1 && touch $DIR1/$FIL2 && touch $DIR1/$FIL3 && x deleteMe"
printcmd "&& exits after error:" "cat $DIR1/$FIL2 && x && ls -al && echo you won\'t see me on the screen"
printcmd "|| will continue until a correct command:" "x || y || z || x || ls || echo testing"
printcmd "commands execute with errors for ;:" "ls; x; y; echo error for x and y;"
printcmd "one command:" "pwd"
printcmd "comments:" "ls -al ; #ls ; pwd"

