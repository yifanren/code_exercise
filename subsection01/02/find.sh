#!/bin/bash

total=$(wc -c $(find $1 -type f)| awk '{print $1}' | awk 'END{print}')

if find $1 -size +1M
then  
    more1M=$(awk 'END{print NR}' $(find $1 -size +1M))

else
then
    more1M=0
fi

printf "all file has $total Byte\n"
printf "has $more1M more than 1M\n"
