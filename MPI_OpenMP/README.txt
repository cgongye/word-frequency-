This code counts English word frequncy of "filename.txt" in the same directory.

For running code on Discovery Cluster
	Compile: mpiCC -fopenmp -o word_frequncy_MPI_OMP word_frequncy_MPI_OMP.cpp
	Run: bsub < run.bash (you need to modify run.bash)
