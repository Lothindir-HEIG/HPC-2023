#!/bin/sh

LOOPS=1
GREP_STRING="M*/s"

while getopts :t:W:i:f:g:a flag; do
   case "$flag" in
   t) TEST=$OPTARG ;;
   W) WORKGROUP=$OPTARG ;;
   i) LOOPS=$OPTARG ;;
   f) FILE_OUTPUT=$OPTARG ;;
   g) GREP_STRING=$OPTARG ;;
   a) APPEND=1 ;;
   *)
      echo "Usage: $0 -t <TEST> -W <thread_domain>:<size>[:<num_threads>[:<chunk size>:<stride>]] -g <GREP_STRING> [-i <LOOPS>] [-f <OUTPUT_FILE>] [-a]"
      exit 1
      ;;
   esac
done

if [ -n $FILE_OUTPUT ]; then
    if [ -z $APPEND ]; then
        > $FILE_OUTPUT
    fi
    for i in $(seq 1 $LOOPS); do
        likwid-bench -t $TEST -W $WORKGROUP 2>/dev/null | grep "$GREP_STRING" >> $FILE_OUTPUT
    done
else
    for i in $(seq 1 $LOOPS); do
        likwid-bench -t $TEST -W $WORKGROUP 2>/dev/null | grep "$GREP_STRING"
    done
fi
