#!/bin/bash
for((i = 1; i < 31; i++)) 
do
    ./dtes 1 $i 0.06 0.01
    ./dtes 3 $i 0.06 0.01
done






