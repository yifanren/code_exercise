#!/bin/bash


if [ $1 = "--clean" ]
then

	rm *.bin

else

head=0;
second=1;

for i in $(seq 1 40);
do
	#chuangjianwenjian
	name=$(printf %04d.bin $i);
	touch $name;
	
	#shezhiwenjiandaxiao
	src=$[$head+$second]
	echo $src
	dd if=/dev/zero of=$name bs=$second count=1
	temp=$second
	second=$src
	head=$temp
		

done

fi



