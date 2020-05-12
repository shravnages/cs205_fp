// CS 205 Final Project
// Given a matrix as a CSV file, calculates the Gram matrix X^T * X

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

const int ROOT = 0;

/**
 * Initializes a square 2D array of size n
 */
float** init_matrix(int r, int c) {
	float **matrix;
	int i1;
	
	matrix = (float **) malloc(sizeof(float *) * r);
	for (i1 = 0; i1 < r; i1++) {
		matrix[i1] = (float *) malloc(sizeof(float) * c);
	}
	return matrix;
}

/**
 * Print out a matrix of a given size n to the console
 */
void print_matrix(float** matrix, int r, int c) {
	int i1, i2;
	for (i1 = 0; i1 < r; i1++) {
		for (i2 = 0; i2 < c; i2++) {
			printf("%f ", matrix[i1][i2]);
		}
		printf("\n");
	}
}

/**
 * Frees up the memory of a given square 2D matrix of size n
 */
void free_matrix(int** matrix, int r) {
	int i1;
	for (i1 = 0; i1 < r; i1++) {
		free(matrix[i1]);
	}
	free(matrix);
}

int main(int argc, char *argv[]) {
	//Declare all the variables we need
	int rank, size;

	char *input;
	char *delim;

	int r, c;
	float **x;
	float **xt_x;

	FILE *file;
	int s;

	int i1, i2, i3;
	int node;

	//The input file and matrix dimensions should be given as command line arguments
	if (argc != 4) {
		printf("ERROR: give the input file and matrix dimension as the first input");
		return 0;
	}

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	input = argv[1];
	r = atoi(argv[2]), c = atoi(argv[3]);

	//Initialize the 2D array to store the matrix
	x = init_matrix(r, c);

	if (rank == 0) {
		//Initialize the input file
		file = fopen(input, "r");
		if (file == NULL) {
			printf("ERROR: could not open input file");
			return 0;
		}

		//Read in the matrix
		for (i1 = 0; i1 < r; i1++) {
			for (i2 = 0; i2 < c; i2++) {
				s = fscanf(file, "%f,", &x[i1][i2]);
				if (s != 1) {
					printf("ERROR: could not read input file");
					return 0;
				}
			}
		}
	}

	//Broadcast each row of the matrix to all nodes
	for (i1 = 0; i1 < r; i1++) {
		MPI_Bcast(x[i1], c, MPI_INT, ROOT, MPI_COMM_WORLD);
	}

	//Calculate X^T * X
	//With some clever indexing, we don't need to actually compute X^T and
	//waste memory, we can do it all with just X.
	xt_x = init_matrix(c, c);
	for (i1 = rank; i1 < c; i1 += size) {
		for (i2 = 0; i2 < c; i2++) {
			for (i3 = 0; i3 < r; i3++) {
				xt_x[i1][i2] += x[i3][i1] * x[i3][i2];
			}
		}
	}

	if (rank == 0) {
		//Go through each node and receive the columns
		for (node = 1; node < size; node++) {
			//Go through each column that this node processed
			for (i1 = node; i1 < c; i1 += size) {
				MPI_Recv(xt_x[i1], c, MPI_INT, node, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}

		//Wait for everything to catch up
		//MPI_Barrier(MPI_COMM_WORLD);
	} else {
		//Send the columns that this node processed to the master node
		for (i1 = rank; i1 < c; i1 += size) {
			MPI_Send(xt_x[i1], c, MPI_INT, ROOT, 0, MPI_COMM_WORLD);
		}

		//Wait for everything to catch up
		//MPI_Barrier(MPI_COMM_WORLD);
	}

	//Print out the resulting matrix on the master node
	if (rank == 0) {
		print_matrix(xt_x, c, c);
	}

	MPI_Finalize();

	return 0;
}
