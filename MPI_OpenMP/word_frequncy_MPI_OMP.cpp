#include <stdio.h>		// use stdio.h instead of iostream 
#include <algorithm>    // generate
#include <vector>       // vector
#include <ctime>        // time
#include <math.h>		// divide power
#include "mpi.h"		// MPI header
#include <iostream>     // cout
#include <map>			// map
#include <fstream>		// ifstream
#include <string>		// string
#include <iomanip>		// io
#include <sstream>		// string stream
#include <omp.h>		// openmp header
#include <algorithm>
using namespace std;


int main (int argc,char **argv)
{
	int rank, NP;
	double start_time, end_time, w_time;
	char * buffer;
	int length;
	int interval;
	map<string, int> wordCount;
	
	// read file
	ifstream input;
	input.open("filename.txt");	
	// get length of file
	input.seekg (0, input.end);
	length = input.tellg();
	input.seekg (0, input.beg);

	// allocate memory:
	buffer = new char [length];
	
	// read data as a block
	input.read (buffer, length);
	input.close();
	
	// MPI section
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Barrier(MPI_COMM_WORLD);
	start_time = MPI_Wtime();
	
	MPI_Comm_size(MPI_COMM_WORLD, &NP);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// send data to all slaves
	interval = length / NP + 1;
	char * subbuffer = new char[interval];
	MPI_Scatter(buffer, interval, MPI_BYTE, subbuffer, interval, MPI_BYTE, 0, MPI_COMM_WORLD);
	
	string temp;
	int convert;
	string word = "";
	map<string, int> subwordCount;
	char breaked_word[30];
	
	// recive breaked word
	if (rank != 0) {
		MPI_Recv(&breaked_word, 30, MPI_BYTE, rank - 1, 0, MPI_COMM_WORLD, &status);
		int x = 0;
		while (breaked_word[x] != NULL) {
			temp = breaked_word[x];
			word.append(temp);
			x++;
		}
	}
	
	// send breaked word
	string sword = "";
	if (rank < (NP -1)) {
		int y = interval - 1;
		convert = subbuffer[y];
		temp = subbuffer[y];
		while (((convert >= 65) && (convert <= 90)) || ((convert >= 97) && (convert <= 122))) {
			sword.append(temp);
			y--;
			convert = subbuffer[y];
			temp = subbuffer[y];
		}
		reverse(sword.begin(),sword.end());
		strcpy(breaked_word, sword.c_str());
		char *b = (char *)malloc(NP * 30 * sizeof(char) + MPI_BSEND_OVERHEAD);
		MPI_Buffer_attach( b, NP * 30 * sizeof(char) + MPI_BSEND_OVERHEAD );
		MPI_Bsend(&breaked_word, 30, MPI_BYTE, rank + 1, 0, MPI_COMM_WORLD);
	}
	// count word into sub map
	MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i < interval; i++) {
		if (subbuffer[i] != NULL) {
			convert = subbuffer[i];
			temp = subbuffer[i];
			if (((convert >= 65) && (convert <= 90)) || ((convert >= 97) && (convert <= 122))) {
				word.append(temp);
			}
			else if (word != "") {
				++subwordCount[word];
				word = "";
			}				
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	
	// collect data back to host
	int num_word = 0;	//number of word in host
	int temp_sum = 0;	//result of reduce sum of each word
	int sub_count = 0;	//number of word in each process
	char boadcast[30];	//brodcast each word in host
	map<string, int>::iterator sit = subwordCount.begin();
	if (rank == 0) {
		num_word = subwordCount.size();
	}
	MPI_Bcast(&num_word, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	
	for (int i = 0; i < num_word; i++) {
		if (rank == 0) {
			strcpy(boadcast, (sit->first).c_str());
		}
		MPI_Bcast(boadcast, 30, MPI_BYTE, 0, MPI_COMM_WORLD);
		string r_boadcast = "";
		
		int m = 0;
		while (boadcast[m] != NULL) {
			temp = boadcast[m];
			r_boadcast.append(temp);
			m++;
		} // r_boadcast = word in host
		sub_count = subwordCount[r_boadcast];

		MPI_Barrier(MPI_COMM_WORLD);
		// reduce sum of each word in host
		MPI_Reduce(&sub_count, &temp_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		
		MPI_Barrier(MPI_COMM_WORLD);
					
		if (rank == 0) {
			wordCount[sit->first] = temp_sum;
			//cout << temp_sum << endl;
			sit++;
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0)
	{
		end_time = MPI_Wtime();
		w_time = end_time - start_time;
		cout << w_time <<endl;
		
		//print
		int a = wordCount.size();
		map<string, int>::iterator it = wordCount.begin();
		#pragma omp parallel for ordered
		for (int i = 0; i < a; i++)	{
			#pragma omp ordered
			printf("%s : %d\n", (it->first).c_str(), it->second);
			++it;
		}
		delete[] buffer;
	} 
	
	delete[] subbuffer;
	MPI_Finalize();
	
	return 0;

}
