#!/bin/sh
#BSUB -J Cheng_Hello
#BSUB -o output_file
#BSUB -e error_file
#BSUB -n 1
#BSUB -q ht-10g
#BSUB cwd /home/gongye.c/Project/OpenMP
work=/home/gongye.c/Project/OpenMP


cd $work


echo 10 threads
export OMP_NUM_THREADS=10
for i in `seq 1 20`
do
	word_frequncy_OpenMP
done
