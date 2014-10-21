#!/bin/bash


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
    ls            -a         -l    ;    pwd    ;
#shows command works with spaces between command and arguements without connector
ls        -al

#shows comments are ignored
clear; echo this is a #test
#shows comments are
ls -al #; ls; pwd

#
echo spacing               is              large  not with echo


exit

