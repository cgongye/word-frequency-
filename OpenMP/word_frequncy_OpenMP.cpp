#include <iostream>     // cout
#include <map>			// map
#include <fstream>		// ifstream
#include <ctime>        // time
#include <string>		// string
#include <vector>       // vector
#include <iomanip>		// io
#include <sstream>		// string stream
#include <omp.h>		// openmp header

using namespace std;



int main()
{
	clock_t start, end;
	double cpu_time;
	start = clock();
	
	map<string, int> wordCount;
	double wall_time = omp_get_wtime();
	ifstream input;
	input.open("filename.txt");
	string word;
	string temp;
	int convert;
	// get length of file
	input.seekg (0, input.end);
	int length = input.tellg();
	input.seekg (0, input.beg);
//	printf("length is %d",length);
	// allocate memory:
	char * buffer = new char [length];
	
	// read data as a block
	input.read (buffer, length);
	
	input.close();
	word = "";
	#pragma omp parallel for private(word, convert, temp) lastprivate(wordCount)
	for (int i = 0; i < length; i++) {
		convert = buffer[i];
		temp = buffer[i];
		if (((convert >= 65) && (convert <= 90)) || ((convert >= 97) && (convert <= 122))) {
			word.append(temp);
		}
		else {
			#pragma omp atomic
			++wordCount[word];
			word = "";
		}
	}
	// free memory
	delete[] buffer;
	double read_time = omp_get_wtime()-wall_time;
	int a = wordCount.size();
	map<string, int>::iterator it = wordCount.begin();
	#pragma omp parallel for ordered
	for (int i = 0; i < a; i++)	{
		#pragma omp ordered
		printf("%s : %d\n", (it->first).c_str(), it->second);
		++it;
	}

 	printf("The number of words is %d.\n",a);
	printf("Reading time used: %f\n", read_time);
	printf("Toltal time used: %f\n", omp_get_wtime()-wall_time);
//	printf("%f\n", omp_get_wtime()-wall_time);
	
}
