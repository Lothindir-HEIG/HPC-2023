#!/bin/bash

# This script is used to test the timing of the a_star executable

SIZES=( 100 200 250 300 500 1000 2500 )

for size in ${SIZES[@]}; do
    echo "Struct - size $size" >> time.log
    /usr/bin/time -a -p -o time.log ./a_star struct $size $size input$size >> astar.log

    echo "Tab - size $size" >> time.log
    /usr/bin/time -a -p -o time.log ./a_star tab $size $size input$size >> astar.log
done

cp time.log time.log.bck
cp astar.log astar.log.bck
