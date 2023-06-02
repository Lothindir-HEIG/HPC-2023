#!/bin/bash

# This script is used to test the timing of the a_star executable

SIZES=( 100 200 250 300 500 1000 2500 )

for size in ${SIZES[@]}; do
    echo "Struct - size $size" >> ../logs/time.log
    /usr/bin/time -a -p -o ../logs/time.log ./a_star struct $size $size input$size >> ../logs/astar.log

    echo "Tab - size $size" >> ../logs/time.log
    /usr/bin/time -a -p -o ../logs/time.log ./a_star tab $size $size input$size >> ../logs/astar.log
done

cp ../logs/time.log ../logs/time.log.bck
cp ../logs/astar.log ../logs/astar.log.bck
