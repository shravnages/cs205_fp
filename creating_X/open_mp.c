#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
// #include "timing.h"
#define NUM_LINES 4

int main (int argc, char *argv[])
{
int	i, j, k;
int DICT_LENGTH = 0;
// timing_t tstart, tend;
char corpus_test[5000] = "this is the first document this document is the second document and this is the third one is this the first document";
/* Step 1: Creating dictionary */
char *dict[500];
char sentence[5000];
strcpy(sentence, corpus_test);
// printf("%s\n", sentence);
// printf("new line.....\n");
char *word = strtok(sentence, " ");
while (word != NULL)
  {
  // printf("%s\n", word);
  int unique_words = 1;
  #pragma omp for shared(DICT_LENGTH) private(j)
  for(j=0; j<DICT_LENGTH; j++)
    {
    char *dict_word = *(dict + j);
    if (strcmp(word, dict_word) == 0)
      {
        // printf("Duplicate! %s\n", dict_word);
      unique_words = 0;
      break;
      }
    }
  if (unique_words == 1)
    {
    // printf("%d\n", DICT_LENGTH);
    // printf("%s\n", word);
    dict[DICT_LENGTH] = word;
    DICT_LENGTH++;
    }
  // printf("%d\n", DICT_LENGTH);
  word = strtok(NULL, " ");
  }

printf("New dict\n");
for (int k = 0; k < DICT_LENGTH; k++)
  {
  char *print_dict_word = *(dict + k);
  printf("%s\n", print_dict_word);
  }
printf("Dictionary length: %d\n", DICT_LENGTH);
printf("done\n");

/* Step 2: Creating X Matrix */
const char *corpus[50] = {"this is the first document",
        "this document is the second document",
        "and this is the third one",
        "is this the first document"};
const char* dictionary[] = {"and", "document", "first", "is", "one", "second", "the", "third", "this"};

int X[NUM_LINES][DICT_LENGTH];


  for (i=0; i<NUM_LINES; i++)
    for (j=0; j<DICT_LENGTH; j++)
      X[i][j]= 0;


  // get_time(&tstart);
  #pragma omp parallel shared(corpus, dictionary, NUM_LINES) private(i)
  for (i=0; i<NUM_LINES; i++)
    {
    char sentence[1000];
    strcpy(sentence, corpus[i]);
		char *word = strtok(sentence, " ");
    // printf("new line.....\n");
		while (word != NULL)
      {
      #pragma omp for shared(DICT_LENGTH) private(j)
  		for(j=0; j<DICT_LENGTH; j++)
    		{
    		char *dict_word = *(dictionary + j);
        // printf("Dictword: %s\n", dict_word);
				// printf("Word: %s\n", word);
				if (strcmp(word, dict_word) == 0)
				  {
          // printf("yes\n");
          // printf("%d %d \n", i, j);
					X[i][j] += 1;
				  }
        else
          {
          // printf("no\n");
          }
		    }
        word = strtok(NULL, " ");
	    }
      printf("%s\n", corpus[i]);
    }
    // get_time(&tend);

    /* Step 3: Creating X Matrix */


  printf("*****************************************************\n");

  for (i=0; i<NUM_LINES; i++) {
      for (j=0; j<DICT_LENGTH; j++) {
          printf("%d, ", X[i][j]);
      }
      printf("\n");
  }
  // printf ("Elapsed Time: %g s.\n", timespec_diff(tstart,tend));
}
