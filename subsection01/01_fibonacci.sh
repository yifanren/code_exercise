#!/bin/bash


if [ $1 = "--clean" ]
then

	rm *.bin

else

head=0;
second=1;

for i in $(seq 1 45);
do
	#creat a file
	name=$(printf %04d.bin $i);
	touch $name;
	
	#set flie number
	src=$[$head+$second]
	echo $src
	dd if=/dev/zero of=$name bs=$second count=1
	temp=$second
	second=$src
	head=$temp
		

done

fi



