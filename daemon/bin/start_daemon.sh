#!/bin/bash

for i in {0..100..10}
do
    mkdir ser$i
    cd ./ser$i
    ../salamandre-daemon -s `expr 2000 + $i` -g `expr 2000 + $i + 1` &
    cd ..
done
