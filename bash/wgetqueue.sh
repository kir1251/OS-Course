#!/bin/bash

shopt -s nullglob

QPATH=$HOME/.queue

mkdir -p "$QPATH"
mkdir -p "$QPATH/requests"

for a in "$@"
do
if [ $a == 'kill' ]; then
    if [ -f $QPATH/pid ]; then
      pid=`cat $QPATH/pid`
      kill $pid      
    fi
    exit 0
  fi
  r=`mktemp --tmpdir="$QPATH/requests"`
  echo "$a" > "$r"
done

if [ -f $QPATH/pid ]; then
  pid=`cat $QPATH/pid`
  if [ ! -e /proc/$pid ]; then 
    rm $QPATH/pid
  fi
else  
  echo $$ > $QPATH/pid
  trap "rm $QPATH/pid" 0
  trap "" SIGHUP
  #exec < /dev/null
  #exec > $QPATH/log
  #exec 2> $QPATH/log
 
  while [ 1 ]
  do
    for a in "$QPATH/requests"/*
    do
      url=`cat $a`
      rm "$a"
      wget -c "$url"
    done
    
    sleep 1
  done
fi
