This code counts English word frequncy of "filename.txt" in the same directory.

For running code on Discovery Cluster
	Compile: mpiCC -o word_frequncy_MPI word_frequncy_MPI.cpp
	Run: bsub < run.bash (you need to modify run.bash)
