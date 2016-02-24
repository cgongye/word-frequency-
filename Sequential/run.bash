#!/bin/sh
#BSUB -J run
#BSUB -o output_file
#BSUB -e error_file
#BSUB -n 1
#BSUB -q interactive-10g
#BSUB cwd /home/gongye.c/Project/Sequential
work=/home/gongye.c/Project/Sequential
cd $work
word_frequncy_sequential
