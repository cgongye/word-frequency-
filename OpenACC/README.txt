uthash is a third party hash libary and it needs to be in the same directory of the code when compiling.

GPU node: bsub -Is -q par-gpu -n 1 -R span[ptile=32] /bin/bash
		  bsub -Is -q par-gpu-2 -n 1 -R span[ptile=48] /bin/bash
		  
Compile: gcc word_frequncy_OpenACC.c -fopenacc -foffload=nvptx-none -foffload="-O3" -O3 -o word_frequncy_OpenACC.x
Run: ./word_frequncy_OpenACC.x > out.txt
