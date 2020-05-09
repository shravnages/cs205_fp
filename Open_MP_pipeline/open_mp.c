#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
// #include "timing.h"
#define NUM_LINES 29
#define DICT_LENGTH 20

int main (int argc, char *argv[])
{
int	i, j, k;
// int DICT_LENGTH = 0;
// timing_t tstart, tend;
// const char *corpus[50] = {"this is the first document",
//         "this document is the second document",
//         "and this is the third one",
//         "is this the first document"};
// char corpus_test[5000] = "this is the first document this document is the second document and this is the third one is this the first document";
// /* Step 1: Creating dictionary */
//
// // Flatten corpus
//
// char *dictionary[500];
//
// for (i = 0; i < NUM_LINES; i++)
//   {
//   printf("Line: %s\n", corpus[i]);
//   char sentence[5000];
//   strcpy(sentence, corpus[i]);
//   // printf("%s\n", sentence);
//   // printf("new line.....\n");
//   char *word = strtok(sentence, " ");
//   while (word != NULL)
//     {
//     printf("Word: %s\n", word);
//     int unique_words = 1;
//     #pragma omp for shared(DICT_LENGTH) private(j)
//     for(j=0; j<DICT_LENGTH; j++)
//       {
//       char *dict_word = dictionary[j];
//       if (strcmp(word, dict_word) == 0)
//         {
//         printf("Duplicate! %s\n", dict_word);
//         unique_words = 0;
//         break;
//         }
//       }
//     if (unique_words == 1)
//       {
//       // printf("%d\n", DICT_LENGTH);
//       // printf("%s\n", word);
//       dictionary[DICT_LENGTH] = word;
//       DICT_LENGTH++;
//       }
//       printf("New dict\n");
//       for (int k = 0; k < DICT_LENGTH; k++)
//         {
//         char *print_dict_word = dictionary[k];
//         printf("%s\n", print_dict_word);
//         }
//       printf("done\n");
//       printf("Dictionary length: %d\n", DICT_LENGTH);
//     // printf("%d\n", DICT_LENGTH);
//     word = strtok(NULL, " ");
//     }
//   }

/* Step 2: Creating X Matrix */

const char *corpus[50] = {"this is the first document",
        "this document is the second document",
        "and this is the third one",
        "is this the first document"};
// const char* dictionary[] = {"and", "document", "first", "is", "one", "second", "the", "third", "this"};

FILE *file;
char *filename;
char line[1000];

filename = "corpus.txt";
file = fopen(filename, "r");
if (file == NULL) {
  printf("ERROR: could not open input file\n");
  return 1;
}

const char* dictionary[] = {"coronavirus",
"covid19",
"lockdown",
"people",
"health",
"march",
"spread",
"cases",
"pandemic",
"positive",
"quarantine",
"virus",
"government",
"restrictions",
"tested",
"friday",
"state",
"country",
"outbreak",
"social"};

int X[NUM_LINES][DICT_LENGTH];


for (i=0; i<NUM_LINES; i++)
  for (j=0; j<DICT_LENGTH; j++)
    X[i][j]= 0;

// get_time(&tstart);
#pragma omp parallel shared(file, dictionary, NUM_LINES) private(i)
i = 0;
while (fgets(line, 1000, file)) {
  printf("%s\n", line);
  char sentence[1000];
  strcpy(sentence, line);
	char *word = strtok(sentence, " ");
  // printf("new line.....\n");
	while (word != NULL)
    {
    int wordLen = strlen(word);
		for (int i1 = 0; i1 < wordLen; i1++)
      {
    	word[i1] = tolower(word[i1]);
    	if (word[i1] == '\n')
        {
    		word[i1] = '\0';
    	  }
      }
    #pragma omp for shared(DICT_LENGTH) private(j)
		for(j=0; j<DICT_LENGTH; j++)
  		{
  		char *dict_word = *(dictionary + j);
      // printf("Dictword: %s\n", dict_word);
			// printf("Word: %s\n", word);
			if (dict_word != NULL && strcmp(word, dict_word) == 0)
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
    // printf("%s\n", corpus[i]);
    i += 1;
  }
  // get_time(&tend);

/* Step 3: Creating X Matrix */
int xt_x[DICT_LENGTH][DICT_LENGTH];
int i1, i2, i3;
for (i1 = 0; i1 < DICT_LENGTH; i1++) {
  for (i2 = 0; i2 < DICT_LENGTH; i2++) {
	    xt_x[i1][i2] = 0;
  }
}

#pragma omp parallel for shared(xt_x) private(i1, i2, i3)
for (i1 = 0; i1 < DICT_LENGTH; i1++) {
	for (i2 = 0; i2 < DICT_LENGTH; i2++) {
		for (i3 = 0; i3 < NUM_LINES; i3++) {
			xt_x[i1][i2] += X[i3][i1] * X[i3][i2];
		}
	}
}


  printf("*****************************************************\n");

  for (i=0; i<NUM_LINES; i++) {
      for (j=0; j<DICT_LENGTH; j++) {
          printf("%d, ", X[i][j]);
      }
      printf("\n");
  }

  printf("*****************************************************\n");

  for (i=0; i<DICT_LENGTH; i++) {
      for (j=0; j<DICT_LENGTH; j++) {
          printf("%d, ", xt_x[i][j]);
      }
      printf("\n");
  }
  // printf ("Elapsed Time: %g s.\n", timespec_diff(tstart,tend));
}
