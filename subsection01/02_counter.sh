#!/bin/bash

function search_File()
{
	for file in $1/*
	do
		if test -f $file
		then
			count_Sum=$[$count_Sum+1]
					
                        file_size=$(ls $file -l | awk '{print $5}')
			file_Sum=$[$file_Sum+$file_size]

                        if [ $file_size -gt $[1024*1024] ]
                        then
			count_More=$[$count_More+1]

                        fi
		else
			search_File $file
			
		fi
	done
}

count_Sum=0
count_More=0
file_Sum=0

search_File $1

printf "all file has $file_Sum Byte\n"
printf "has $count_More file more than 1M\n"


