#!/bin/bash

for i in {0..100..10}
do
    mkdir -p ser$i
    cd ./ser$i
    ../salamandre-daemon -s `expr 20000 + $i` -g `expr 20000 + $i + 1` &> log.txt &
    cd ..
done
