#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
// #define NUM_LINES 24102
// #define DICT_LENGTH 20

int main (int argc, char *argv[])
{
  int	i, j, k;
  FILE *file;
  char *filename;
  char line[1000];

  const int NUM_LINES = 362130;
  const int DICT_LENGTH = 20;

  filename = argv[1];
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

  int **X;//[NUM_LINES][DICT_LENGTH];

  X = (int **) malloc(sizeof(int *) * NUM_LINES);
  int i1, i2, i3;
	for (i1 = 0; i1 < NUM_LINES; i1++) {
		X[i1] = (int *) malloc(sizeof(int) * DICT_LENGTH);
	}

  for (i=0; i<NUM_LINES; i++)
    for (j=0; j<DICT_LENGTH; j++)
      X[i][j]= 0;

  i = 0;
  // #pragma omp parallel shared(file, dictionary)
  while (fgets(line, 1000, file)) {
    //printf("Line: %s\n", line);
    char sentence[1000];
    strcpy(sentence, line);
  	char *word = strtok(sentence, " ");
  	while (word != NULL)
      {
      //printf("Word: %s\n", word);
      int wordLen = strlen(word);
      #pragma omp parallel for
  		for (int i1 = 0; i1 < wordLen; i1++)
        {
      	word[i1] = tolower(word[i1]);
      	if (word[i1] == '\n')
          {
      		word[i1] = '\0';
      	  }
        }
      #pragma omp for
  		for(j=0; j<DICT_LENGTH; j++)
    		{
    		char *dict_word = *(dictionary + j);
  			if (dict_word != NULL && strcmp(word, dict_word) == 0 && i < NUM_LINES)
  			  {
          //printf("%d %d \n", i, j);
  				X[i][j] += 1;
          //printf("Set \n");
  			  }
  	    }
        word = strtok(NULL, " ");
      }
      i += 1;
    }

    // printf("*****************************************************\n");

    for (i=0; i<NUM_LINES; i++) {
        for (j=0; j<DICT_LENGTH; j++) {
            printf("%d ", X[i][j]);
        }
        printf("\n");
    }

    for (i1 = 0; i1 < NUM_LINES; i1++) {
  		free(X[i1]);
  	}
  	free(X);
  }
