#!/bin/bash

if [ $1 = "--clean" ]
then
	rm *.bin

else

first=0;
second=1;

for i in $(seq 1 45);
do
	name=$(printf %04d.bin $i);
	dd if=/dev/zero of=$name bs=$second count=1

	temp=$second
	second=$[$first + $second]
        first=$temp
done

fi



