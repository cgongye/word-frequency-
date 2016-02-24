#include <iostream>     // cout
#include <map>			// map
#include <fstream>		// ifstream
#include <ctime>        // time
#include <string>		// string
#include <vector>       // vector
#include <iomanip>
#include <sstream>

struct letter_only: std::ctype<char> 
{
    letter_only(): std::ctype<char>(get_table()) {}

    static std::ctype_base::mask const* get_table()
    {
        static std::vector<std::ctype_base::mask> 
            rc(std::ctype<char>::table_size,std::ctype_base::space);

        std::fill(&rc['A'], &rc['z'+1], std::ctype_base::alpha);
        return &rc[0];
    }
};

int main()
{
	clock_t start, end;
	double cpu_time;
	start = clock();
	
     std::map<std::string, int> wordCount;
     std::ifstream input;
     input.imbue(std::locale(std::locale(), new letter_only())); //enable reading only letters!
     input.open("filename.txt");
     std::string word;
     while(input >> word)
     {
         ++wordCount[word];
     }
     for (std::map<std::string, int>::iterator it = wordCount.begin(); it != wordCount.end(); ++it)
     {
           std::cout << it->first <<" : "<< it->second << std::endl;
     }
	 
	end = clock();
	cpu_time = (double(end - start))/CLOCKS_PER_SEC;
	std::cout << "CPU time used is "<<cpu_time<<std::endl;
}
