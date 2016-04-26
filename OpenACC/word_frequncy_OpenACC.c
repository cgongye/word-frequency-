#include <string.h>  /* strcpy */
#include <stdlib.h>  /* malloc */
#include <stdio.h>   /* printf */
#include "uthash.h"
#include <sys/time.h>
#include <time.h>
struct my_struct {
    char name[30];             /* key */
    int id;
    UT_hash_handle hh;         /* makes this structure hashable */
};
int name_sort(struct my_struct *a, struct my_struct *b) {
    return strcmp(a->name,b->name);
}

int main(int argc, char *argv[])
{
	struct timeval starttime,endtime;
	double runTime = 0.0;
	gettimeofday(&starttime,NULL);

	// read file in a array
	FILE *f = fopen("filename.txt", "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *buffer = malloc(fsize + 1);
	fread(buffer, fsize, 1, f);
	fclose(f);
	buffer[fsize] = 0;

	int convert;
	char temp[1];
	char word[30] = "";
	char emp[30] = "";
	
    const char **n, *names[] = { "joe", "bob", "betty", NULL };
    struct my_struct *s, *tmp, *wordcount = NULL;
    int i=0;
	#pragma acc region
	{
		#pragma acc loop
		for ( i = 0; i < fsize; i++) {
			if (buffer[i] != NULL) {
				convert = buffer[i];
				temp[0] = buffer[i];

				if (((convert >= 65) && (convert <= 90)) || ((convert >= 97) && (convert <= 122))) {
					strncat(word, temp, 1);

				}
				else if (strcmp (word,emp) != 0) {
					HASH_FIND_STR(wordcount, word, s);
					if (s != NULL) {
						s->id += 1;
						strcpy(word, emp);
					}
					else {
						s = (struct my_struct*)malloc(sizeof(struct my_struct));
						strncpy(s->name, word,30);
						s->id = 1;
						HASH_ADD_STR( wordcount, name, s );
						strcpy(word, emp);
					}
				}				
			}
		}
	}

	HASH_SORT(wordcount, name_sort);
    for(s=wordcount; s != NULL; s=s->hh.next) {
        printf("%s : %d\n", s->name, s->id);
    }

    /* free the hash table contents */
    HASH_ITER(hh, wordcount, s, tmp) {
        HASH_DEL(wordcount, s);
        free(s);
    }
	gettimeofday(&endtime,NULL);
	runTime=1000000*(endtime.tv_sec-starttime.tv_sec)+endtime.tv_usec-starttime.tv_usec;
	runTime=runTime/1000000;
	printf("Toltal time used: %f\n", runTime);
    return 0;
}
