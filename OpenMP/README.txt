This code counts English word frequncy of "filename.txt" in the same directory.

For running code on Discovery Cluster
	Compile: g++ -fopenmp -o ~/bin/word_frequncy_OpenMP word_frequncy_OpenMP.cpp
	Run: bsub < run.bash (you need to modify run.bash)
For running on local machine
	Compile: g++ -fopenmp -o word_frequncy_OpenMP word_frequncy_OpenMP.cpp
	Run: ./word_frequncy_OpenMP >> out.txt