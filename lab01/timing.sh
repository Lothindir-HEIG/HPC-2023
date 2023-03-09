#!/bin/bash

# This script is used to test the timing of the a_star executable

SIZES=( 100 200 250 300 500 1000 2500 )

for size in ${SIZES[@]}; do
    time ./a_star struct $size $size input$size >> log.txt
done

for size in ${SIZES[@]}; do
    time ./a_star tab $size $size input$size >> log.txt
done