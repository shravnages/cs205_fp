#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
// #include "timing.h"
#define NUM_LINES 4
#define DICT_LENGTH 9

int main (int argc, char *argv[])
{
int	i, j, k;
// timing_t tstart, tend;
char corpus[][50] = {"This is the first document",
    "This document is the second document",
   "And this is the third one",
    "Is this the first document"};

const char* dictionary[] = {"and", "document", "first", "is", "one", "second", "the", "third", "this"};

//const char** corpus = {"a b", "c a b"};
// char corpus[][6] = {"b a", "c b a"};
//
// const char *dictionary[] = {"a", "b", "c"};

int c[NUM_LINES][DICT_LENGTH];


  for (i=0; i<NUM_LINES; i++)
    for (j=0; j<DICT_LENGTH; j++)
      c[i][j]= 0;


  // get_time(&tstart);
  #pragma omp parallel shared(corpus, dictionary, NUM_LINES) private(i)
  for (i=0; i<NUM_LINES; i++)
    {
		char *word = strtok(corpus[i], " ");
    printf("new line.....\n");
		while (word != NULL)
      {
      #pragma omp for shared(DICT_LENGTH) private()
  		for(j=0; j<DICT_LENGTH; j++)
    		{
    		char *dict_word = *(dictionary + j);
        // printf("Dictword: %s\n", dict_word);
				// printf("Word: %s\n", word);
				if (strcmp(word, dict_word) == 0)
				  {
          printf("yes\n");
          printf("%d %d \n", i, j);
					c[i][j] += 1;
				  }
        else
          {
          printf("no\n");
          }
		    }
        word = strtok(NULL, " ");
	    }
    }
    // get_time(&tend);



  printf("*****************************************************\n");

  for (i=0; i<NUM_LINES; i++) {
      for (j=0; j<DICT_LENGTH; j++) {
          printf("%d, ", c[i][j]);
      }
      printf("\n");
  }
  // printf ("Elapsed Time: %g s.\n", timespec_diff(tstart,tend));
}
