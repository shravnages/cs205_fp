#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#define NUM_LINES 5
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

  int X[NUM_LINES][DICT_LENGTH];
  int xt_x[DICT_LENGTH][DICT_LENGTH];

  int i1, i2, i3;
	for (i1 = 0; i1 < NUM_LINES; i1++) {
		for (i2 = 0; i2 < DICT_LENGTH; i2++) {
      X[i1][i2] = 0;
			int s = fscanf(file, "%d", &X[i1][i2]);
			if (s != 1) {
				printf("ERROR: could not read input file\n");
				return 0;
			}
		}
	}

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

    for (i=0; i<DICT_LENGTH; i++) {
        for (j=0; j<DICT_LENGTH; j++) {
            printf("%d ", xt_x[i][j]);
        }
        printf("\n");
    }
    // printf ("Elapsed Time: %g s.\n", timespec_diff(tstart,tend));
  }
