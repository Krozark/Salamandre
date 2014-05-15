#!/bin/bash

for i in {0..100..10}
do
    mkdir ser$i
    cd ./ser$i
i    ../salamandre-daemon -s `expr 2000 + $i` -g `expr 2000 + $i + 1` &>> log.txt &
    cd ..
done
