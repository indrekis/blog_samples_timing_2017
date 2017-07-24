#!/bin/sh

folders="gcc* clang*"

for fld in $folders 
do
    cd $fld
    for t in check_timeouts_*
    do
	echo "------------------------------------------------------------"
	echo "------------------------------------------------------------"
	echo $fld "   " $t
	
	./$t
    done 
    cd ..
done
