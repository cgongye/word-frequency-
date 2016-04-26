#include <iostream>     // cout
#include <map>			// map
#include <fstream>		// ifstream
#include <ctime>        // time
#include <string>		// string
#include <vector>       // vector
#include <iomanip>
#include <sstream>
#include <omp.h>		// openmp header

#define NUM 13000000
using namespace std;
struct letter_only: ctype<char> 
{
    letter_only(): ctype<char>(get_table()) {}

    static ctype_base::mask const* get_table()
    {
        static vector<ctype_base::mask> 
            rc(ctype<char>::table_size,ctype_base::space);

        fill(&rc['A'], &rc['z'+1], ctype_base::alpha);
        return &rc[0];
    }
};

int main()
{
	double wall_time = omp_get_wtime();
	map<string, int> wordCount;
	ifstream input;
	input.imbue(locale(locale(), new letter_only())); //enable reading only letters!
	input.open("filename.txt");
	string word;
	#pragma omp parallel for ordered shared(input, wordCount) private (word)
	for (int i = 0; i < NUM; i++) {
		#pragma omp ordered
		if (input >> word) {
			#pragma omp atomic
			++wordCount[word];
		}
	}
	
	double read_time = omp_get_wtime()-wall_time;
	
	int a = wordCount.size();
	map<string, int>::iterator it = wordCount.begin();
	#pragma omp parallel for ordered
	for (int i = 0; i < a; i++)	{
		#pragma omp ordered
		printf("%s : %d\n", (it->first).c_str(), it->second);
		++it;
	}

	printf("Reading time used: %f\n", read_time);
	printf("Toltal time used: %f\n", omp_get_wtime()-wall_time);
}
