#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#define NUM_LINES 24102
#define DICT_LENGTH 20

int main (int argc, char *argv[])
{
  int	i, j, k;
  FILE *file;
  char *filename;
  char line[1000];

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

  int X[NUM_LINES][DICT_LENGTH];


  for (i=0; i<NUM_LINES; i++)
    for (j=0; j<DICT_LENGTH; j++)
      X[i][j]= 0;

  #pragma omp parallel shared(file, dictionary, NUM_LINES) private(i)
  i = 0;
  while (fgets(line, 1000, file)) {
    printf("%s\n", line);
    char sentence[1000];
    strcpy(sentence, line);
  	char *word = strtok(sentence, " ");
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
  			if (dict_word != NULL && strcmp(word, dict_word) == 0)
  			  {
  				X[i][j] += 1;
  			  }
  	    }
        word = strtok(NULL, " ");
      }
      i += 1;
    }

    printf("*****************************************************\n");

    for (i=0; i<NUM_LINES; i++) {
        for (j=0; j<DICT_LENGTH; j++) {
            printf("%d ", X[i][j]);
        }
        printf("\n");
    }
  }
