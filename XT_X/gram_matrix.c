#include <stdio.h>
#include <stdlib.h>

/**
 * Initializes a square 2D array of size n
 */
int** init_matrix(int r, int c) {
	int** matrix = (int **) malloc(sizeof(int *) * r);
	for (int i1 = 0; i1 < r; i1++) {
		matrix[i1] = (int *) malloc(sizeof(int) * c);
	}
	return matrix;
}

/**
 * Print out a matrix of a given size n to the console
 */
void print_matrix(int** matrix, int r, int c) {
	int i1, i2;
	for (i1 = 0; i1 < r; i1++) {
		for (i2 = 0; i2 < c; i2++) {
			printf("%d ", matrix[i1][i2]);
		}
		printf("\n");
	}
}

/**
 * Frees up the memory of a given square 2D matrix of size n
 */
void free_matrix(int** matrix, int r) {
	for (int i1 = 0; i1 < r; i1++) {
		free(matrix[i1]);
	}
	free(matrix);
}

int main(int argc, char *argv[]) {
	//The input file and matrix dimensions should be given as command line arguments
	if (argc != 4) {
		printf("ERROR: give the input file and matrix dimension as the first input");
		return 0;
	}

	char *input = argv[1];
	int r = atoi(argv[2]), c = atoi(argv[3]);

	//Initialize the 2D array to store the matrix
	int **x = init_matrix(r, c);

	//Initialize the input file
	FILE *file;
	file = fopen(input, "r");
	if (file == NULL) {
		printf("ERROR: could not open input file");
		return 0;
	}

	//Read in the matrix
	int i1, i2, s;
	for (i1 = 0; i1 < r; i1++) {
		for (i2 = 0; i2 < c; i2++) {
			s = fscanf(file, "%d", &x[i1][i2]);
			if (s != 1) {
				printf("ERROR: could not read input file");
				return 0;
			}
		}
	}

	//Print out the X matrix
	printf("X =\n");
	print_matrix(x, r, c);

	//Calculate X^T * X
	int **xt_x = init_matrix(c, c);
	int i3;
	for (i1 = 0; i1 < c; i1++) {
		for (i2 = 0; i2 < c; i2++) {
			for (i3 = 0; i3 < r; i3++) {
				xt_x[i1][i2] += x[i3][i1] * x[i3][i2];
			}
		}
	}

	//Print out X^T * X
	printf("\nX^T * X =\n");
	print_matrix(xt_x, c, c);

	return 0;
}