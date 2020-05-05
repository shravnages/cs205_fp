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
// const char** corpus = {"This is the first document.",
//     "This document is the second document.",
//    "And this is the third one.",
//     "Is this the first document?"};
//
// const char** dictionary = {"and", "document", "first", "is", "one", "second", "the", "third", "this"};

const char** corpus = {"a b", "c a b"};

const char** dictionary = {"a", "b", "c"};

int c[NUM_LINES][DICT_LENGTH];


  for (i=0; i<NUM_LINES; i++)
    for (j=0; j<DICT_LENGTH; j++)
      c[i][j]= 0;


  // get_time(&tstart);
  #pragma omp parallel shared(corpus, dictionary, NUM_LINES) private(i)
  for (i=0; i<NUM_LINES; i++)
    {
		#pragma omp for shared(DICT_LENGTH) private()
    for(j=0; j<DICT_LENGTH; j++)
		  {
			char *word = strtok(corpus[i], " ");
			char *dict_word = *(dictionary + j);
      while (word != NULL)
				if (strcmp(tolower(word), dict_word) == 0)
				{
	        c[i][j] += 1;
				};
				word = strtok(NULL, " ");
		  }
    }
    // get_time(&tend);



  printf("*****************************************************\n");

  for (i=0; i<NUM_LINES; i++) {
      for (j=0; j<DICT_LENGTH; j++) {
          printf(c[i][j]);
      }
  }
  // printf ("Elapsed Time: %g s.\n", timespec_diff(tstart,tend));
}
