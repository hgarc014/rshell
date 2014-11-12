#!/bin/bash

# FILE MUST BE RUN FROM MAIN RSHELL FOLDER (where Makefile and LICENESE are)

CMD=./bin/ls
TEST="# testing"
ALLFILES="LICENSE README.md Makefile"
ALLDIRS="bin . .git .. tests src"
HIDDIRS=".git .. ."
DIRS="bin tests src"

# $1 = test message
# $2 = command
function printcmd
{
    echo "$TEST $1 $2"
    $2
}

echo -e "\n#TESTING NO PARAMETERS AND FLAGS\n"

printcmd "no flags:" "$CMD"
printcmd "-a flag:" "$CMD -a"
printcmd "-al flag:" "$CMD -al"
printcmd "spaced flag" "$CMD   -a   -l"
printcmd "-aR flag:" "$CMD -aR"
printcmd "-l flag:" "$CMD -l"
printcmd "-lR flag:" "$CMD -lR"
printcmd "-R flag:" "$CMD -R"
printcmd "all flags:" "$CMD -alR"
printcmd "diff order:" "$CMD -Ral"
printcmd "separated:" "$CMD -a   -R   -l"


echo -e "\n#TESTING DIRS, FILES, AND FLAGS\n"

printcmd "no flags(file):" "$CMD README.md"
printcmd "-a flag(file, dirs, file):" "$CMD -a Makefile $DIRS LICENSE"
printcmd "-al flag(alldirs, files):" "$CMD -al $ALLDIRS LICENSE README.md"
printcmd "-aR flag(dirs and allfiles):" "$CMD -aR $DIRS $ALLFILES"
printcmd "-l flag:(dir, allfiles, dir)" "$CMD -l bin $ALLFILES src"
printcmd "-lR flag:(files)" "$CMD -lR $ALLFILES"
printcmd "-R flag:(dir)" "$CMD -R bin"
printcmd "all flags:(DIRS)" "$CMD -alR $DIRS"

echo -e "\n#TESTING PATHS\n"

printcmd "*:" "$CMD *"
printcmd "~:" "$CMD ~"
printcmd "going back:" "$CMD ../"
printcmd "back twice:" "$CMD  ../.."
printcmd "previous fold:" "$CMD ../rshell"
printcmd "previous and files in cur" "$CMD ../ src bin"
printcmd "previous and folders" "$CMD ../ ../ucr-cs100"
 
#not valid uses
echo -e "\n#SHOWING INVALID USES\n"

printcmd "invalid flags with valid flags:" "$CMD -aewrlf"
printcmd "invalid flags:" "$CMD -uop"
printcmd "no flag with dash:" "$CMD -"

#printcmd "" ""


