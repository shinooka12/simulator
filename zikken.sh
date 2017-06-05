#!/bin/bash

command="rm result.txt"
command1="./sim_p2p >> result.txt"
command2="./result_ave"

eval $command
echo "running now.........."
echo "stage    start####################end"
echo -n "progress start"

for i in `seq 100`
do

eval $command1
flag=`expr $i % 5`
if test ${flag} -eq 0
then
echo -n "#"
fi

done
echo "end"

eval $command2
