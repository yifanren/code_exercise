#!/bin/bash

function SearchFile()
{
    for file in $1/*
    do
        if test -f $file
        then
            countSum=$[$countSum+1]
            filesize=$(ls $file -l | awk '{print $5}')
            fileSum=$[$fileSum+$filesize]

            if [ $filesize -gt $[1024*1024] ]
                then
                countMore=$[$countMore+1]
                fi
                
            else
                if test -d $file
                then
                    SearchFile $file
                fi
        fi
    done
}

countSum=0
countMore=0
fileSum=0

SearchFile $1

printf "all file has $fileSum Byte\n"
printf "has $countMore file more than 1M\n"


