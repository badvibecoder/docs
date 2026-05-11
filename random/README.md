# Random stuff here



pwd
    /home/name
mydir=$(pwd)
echo $mydir
    /home/name

strip /home/ and isolate name

echo $mydir | awk -F/ '{print $NF}'
    name

this takes about 2ms

basename $mydir
    name

this effectively does the same thing 1ms

Doing it in bash is faster

echo ${mydir##*/}
    name

this is sub ms


echo $0
    bash
echo ${0^^}
    BASH
echo ${0^}
    Bash



