#!/bin/bash

while [[ $# -gt 0 ]]
do
  case $1 in
    -iname)
      shift 1
      iname=$1
      shift 1
      ;;
    -type)
      shift 1
      if [[ ! $1 == [dfbcls] ]]; then
        echo "find: Arguments to -type should contain only one letter"
        exit
      fi
      filetype=$1
      shift 1
  esac
done

rFind()
{
  if [[ ($iname == ${1##*/}) && ( -e $file ) && (($filetype == "") || 
        ( $filetype == 'd' && -d $file ) || 
        ( $filetype == 'f' && -f $file ) ||
        ( $filetype == 'b' && -b $file ) ||
        ( $filetype == 'c' && -c $file ) ||
        ( $filetype == 'l' && -L $file ) ||
        ( $filetype == 's' && -S $file )) ]]; then
    echo $1   
#    echo $iname ${1##*/} 
  fi
  if [[ -d $1 ]]; then
    for file in $1/*
    do      
      rFind $file     
    done
  fi
}
rFind ./
