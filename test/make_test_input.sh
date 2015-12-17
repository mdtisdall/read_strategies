#!/bin/bash

  head -c 44M /dev/urandom > inputfile_0
for i in `seq 1 255`;
do
  cp inputfile_0 inputfile_$i
done 
