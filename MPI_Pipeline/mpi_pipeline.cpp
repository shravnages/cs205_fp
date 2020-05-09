#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

#include <string.h>
#include <ctype.h>

#include <mpi.h>

using namespace std;

const int ROOT = 0;
const int MAX_LINE_LENGTH = 1000;

/**
 * Create a 2D integer matrix with r rows and c columns
 */
int** init_matrix(int r, int c) {
	int **matrix;
	int i1, i2;
	
	matrix = (int **) malloc(sizeof(int *) * r);
	for (i1 = 0; i1 < r; i1++) {
		matrix[i1] = (int *) malloc(sizeof(int) * c);
		for (i2 = 0; i2 < c; i2++) {
			matrix[i1][i2] = 0;
		}
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
			cout << matrix[i1][i2] << " ";
		}
		cout << endl;
	}
}

/**
 * Frees up the memory of a given square 2D matrix with r rows
 */
void free_matrix(int **matrix, int r) {
	int i1;
	for (i1 = 0; i1 < r; i1++) {
		free(matrix[i1]);
	}
	free(matrix);
}

/**
 * Get the index of a given element in a vector
 */
int indexOf(vector<string> dictionary, char word[1000]) {
	vector<string>::iterator it = find(dictionary.begin(), dictionary.end(), word);
	if (it == dictionary.end()) {
		return -1;
	}
	return distance(dictionary.begin(), it);
}

int main(int argc, char *argv[]) {
	int rank, size;
	FILE *file;
	char *filename;
	int s;
	int fileCounter;

	FILE *dictionaryFile;
	FILE *matrixFile;

	char *word;
	int wordLen;

	char line[1000];
	char *fgetsres;

	int **x;
	int **xt_x;

	int i1, i2, i3;
	int r, c;
	int index;
	int node;
	int t;

	//Setup MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (argc != 2) {
		cout << "ERROR: give the input file as a command line argument" << endl;
		return 1;
	}

	filename = argv[1];

	//Init dictionary on all nodes
	vector<string> dictionary;

	//Create file on all nodes
	file = fopen(filename, "r");
	if (file == NULL) {
		printf("ERROR: could not open input file\n");
		return 1;
	}

	//Keep track of how long the file is
	fileCounter = 0;

	//Loop over every row in the file
	while (fgets(line, 1000, file)) {
		//Split the line on spaces
		word = strtok(line, " ");
		while (word != NULL) {
			//Convert word to lowercase and strip off trailing \n
			wordLen = strlen(word);
			for (i1 = 0; i1 < wordLen; i1++) {
				word[i1] = tolower(word[i1]);
				if (word[i1] == '\n') {
					word[i1] = '\0';
				}
			}

			//If it is not in the dictionary add it
			if (find(dictionary.begin(), dictionary.end(), word) == dictionary.end()) {
				dictionary.push_back(word);
			}

			word = strtok(NULL, " ");
		}

		fileCounter++;
	}

	fclose(file);

	if (rank == 0) {
		cout << "Dictionary size: " << dictionary.size() << endl;
	}

	//Wait for all nodes to catch up
	MPI_Barrier(MPI_COMM_WORLD);

	//Create the X matrix
	r = fileCounter;
	c = dictionary.size();
	x = init_matrix(r, c);

	fileCounter = 0;

	//Re-init file
	file = fopen(filename, "r");

	//Offset by the rank so each node only processes it's own lines
	for (i1 = 0; i1 < rank; i1++) {
		fgetsres = fgets(line, 1000, file);
		fileCounter += 1;
	}

	//Read through the file and build the X matrix for every line
	while (fgets(line, 1000, file)) {
		word = strtok(line, " ");
		while (word != NULL) {
			wordLen = strlen(word);
			for (i1 = 0; i1 < wordLen; i1++) {
				word[i1] = tolower(word[i1]);
				if (word[i1] == '\n') {
					word[i1] = '\0';
				}
			}

			//Get the index in the dictionary
			index = indexOf(dictionary, word);
			x[fileCounter][index] += 1;

			word = strtok(NULL, " ");
		}

		fileCounter += 1;
		
		//Skip to the next line we should process on this node
		for (i1 = 0; i1 < size - 1; i1++) {
			fgetsres = fgets(line, 1000, file);
			fileCounter += 1;
		}
	}

	fclose(file);

	if (rank == 0) {
		//Go through each node and receive the rows they processed
		for (node = 1; node < size; node++) {
			for (i1 = node; i1 < r; i1 += size) {
				MPI_Recv(x[i1], c, MPI_INT, node, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
	} else {
		//Send each row to the root node
		for (i1 = rank; i1 < r; i1 += size) {
			MPI_Send(x[i1], c, MPI_INT, ROOT, 0, MPI_COMM_WORLD);
		}
	}

	//Broadcast the full X matrix to each node
	for (i1 = 0; i1 < r; i1++) {
		MPI_Bcast(x[i1], c, MPI_INT, ROOT, MPI_COMM_WORLD);
	}
	
	//Compute X^T * X
	xt_x = init_matrix(c, c);
	for (i1 = rank; i1 < c; i1 += size) {
		for (i2 = 0; i2 < c; i2++) {
			for (i3 = 0; i3 < r; i3++) {
				xt_x[i1][i2] += x[i3][i1] * x[i3][i2];
			}
		}
	}

	free_matrix(x, r);

	//Wait for all nodes to finish computing
	MPI_Barrier(MPI_COMM_WORLD);

	//Collect X^T * X on the root node
	if (rank == 0) {
		//Go through each node and the rows it processed
		for (node = 1; node < size; node++) {
			for (i1 = node; i1 < c; i1 += size) {
				MPI_Recv(xt_x[i1], c, MPI_INT, node, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
	} else {
		//Send each row this node processed to the root
		for (i1 = rank; i1 < c; i1 += size) {
			MPI_Send(xt_x[i1], c, MPI_INT, ROOT, 0, MPI_COMM_WORLD);
		}
	}

	//Write out the dictionary and matrix to a file
	if (rank == 0) {
		//Write the dictionary
		dictionaryFile = fopen("dictionary.txt", "w");
		if (dictionaryFile == NULL) {
			cout << "Error opening dictionary result file" << endl;
			return 1;
		}
		for (i1 = 0; i1 < dictionary.size(); i1++) {
			fprintf(dictionaryFile, "%s\n", dictionary[i1].c_str());
		}
		fclose(dictionaryFile);

		cout << "Dictionary written to dictionary.txt" << endl;

		//Write the X^T * X matrix
		matrixFile = fopen("cooccur_matrix.txt", "w");
		if (matrixFile == NULL) {
			cout << "Error opening matrix result file" << endl;
			return 1;
		}
		for (i1 = 0; i1 < c; i1++) {
			for (i2 = 0; i2 < c; i2++) {
				fprintf(matrixFile, "%d ", xt_x[i1][i2]);
			}
			fputc('\n', matrixFile);
		}
		fclose(matrixFile);

		cout << "X^T * X matrix written to cooccur_matrix.txt" << endl;
	}

	free_matrix(xt_x, c);

	MPI_Finalize();

	return 0;
}
