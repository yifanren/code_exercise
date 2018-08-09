#!/bin/bash

total=$(wc -c $(find $1 -type f)| awk '{print $1}' | awk 'END{print}')

more1M=$(find $1 -size +1M| awk 'END{print NR}')

printf "all file has $total Byte\n"
printf "has $more1M more than 1M\n"
