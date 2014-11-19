#!/bin/bash

# FILE MUST BE RUN FROM MAIN RSHELL FOLDER (where Makefile and LICENESE are)
# Files end in .rem for easy access to delete all files

TEST="# testing"
CHECK="# checking contents of:"
FILE1=newfile.rem
FILE2=newfile2.rem
FILE3=lower.rem
FILE4=upper.rem
ERROR1=error.rem
FILES1=string.rem

# $1 = test message
# $2 = command
function printcmd
{
    echo -e "\n$TEST $1 $2"
    eval $2
}
function checkcmd
{
    echo "$CHECK $1"
    eval "cat $1"
}

#---------------------------OUTPUT---------------------------
printcmd "redirect:" "echo this is a test > $FILE1"
checkcmd "$FILE1"

printcmd "redirect without spacing:" "echo this is a test>$FILE1"
checkcmd "$FILE1"

printcmd "append:" "echo test number 2 >> $FILE1"
checkcmd "$FILE1"

printcmd "error redirect:" "cat doesntExist 2> $ERROR1"
checkcmd "$ERROR1"

printcmd "error redirect w/out spacing:" "cat doesntExist 2>$ERROR1"
checkcmd "$ERROR1"

printcmd "error append:" "cat cantDoIt 2>> $ERROR1"
checkcmd "$ERROR1"

printcmd "error append w/out spacing:" "cat cantDoIt 2>>$ERROR1"
checkcmd "$ERROR1"


#-----------------------INPUT------------------------------


printcmd "input:" "cat < $FILE1"
checkcmd "$FILE1"

printcmd "input from string:" "cat <<< \"a test from string\"" 

printcmd "input from string + redirect:" "cat <<< \"testing from string\" > $FILES1" 
checkcmd "$FILES1"

printcmd "input from string + redirect:" "cat <<< \"adding to file\" > $FILES1" 
checkcmd "$FILES1"

printcmd "input + redirect:" "cat < $FILE1 > $FILE2"
checkcmd "$FILE2"

printcmd "input + append:" "cat < $FILE1 >> $FILE2"
checkcmd "$FILE2"

printcmd "mult in:" "cat < $FILE1 < $FILE2"
printcmd "mult out:" "cat $FILE1 > $FILE2 > $FILE3"
checkcmd "$FILE2"
checkcmd "$FILE3"


#--------------------------PIPING----------------------------
 
printcmd "piping:" "cat $FILE1 | cat | cat | grep test | grep 2"

printcmd "input,piping:" "cat < $FILE1 | grep test | cat | grep 2"

printcmd "input,piping from string:" "cat <<< \"TESTING OUTPUT\" | grep TEST | cat"

printcmd "piping,output:" "cat $FILE1 | grep test | cat | grep 2| wc > $FILES1"
checkcmd "$FILES1"

printcmd "piping,output append:" "cat $FILE1 | grep test | cat >> $FILES1"
checkcmd "$FILES1"

printcmd "piping,output error:" "cat $FILE1 | grep test | cat | opps 2> $FILES1"
checkcmd "$FILES1"

printcmd "piping,output error append:" "cat $FILE1 | grep test | cat | ouch 2>> $FILES1"
checkcmd "$FILES1"

printcmd "input,piping,output:" "cat < $FILE1 | tr A-Z a-z | tee $FILE3 | tr a-z A-Z > $FILE4"
checkcmd "$FILE3"
checkcmd "$FILE4"

#-------------------------ERRORS/MISUSE----------------------

echo -e "\n\n# Errors/misuse below"

printcmd "input inside piping:" "cat < $FILE1 | cat | cat < $FILE3| cat"
printcmd "ouput inside files:" "cat < $FILE3 | cat | cat > $FILES1 | cat | cat > $FILE4"


