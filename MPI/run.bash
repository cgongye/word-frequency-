#!/bin/sh
#BSUB -J mh
#BSUB -o mpi_output_file
#BSUB -e mpi_error_file
#BSUB -n 16
#BSUB -R "span[ptile=8]"
#BSUB -q ht-10g
#BSUB cwd /home/gongye.c/Project/MPI
work=/home/gongye.c/Project/MPI

cd $work
tempfile1=hostlistrun
tempfile2=hostlist-tcp
echo $LSB_MCPU_HOSTS > $tempfile1
declare -a hosts
read -a hosts < ${tempfile1}
for ((i=0; i<${#hosts[@]}; i += 2)) ;
  do
   HOST=${hosts[$i]}
   CORE=${hosts[(($i+1))]}
   echo $HOST:$CORE >> $tempfile2
done
#####################################################
#####
###DO NOT EDIT ANYTHING ABOVE THIS LINE#########
#####################################################

mpirun -np 16 -prot -TCP -lsf word_frequncy_MPI
