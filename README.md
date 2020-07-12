
# COVID-19 News Article Pipeline
## Harvard University, CS205
## Shravan Nageswaran, Jordan Turley, Shucheng Yan

In this project, my group and I built a pipeline that takes raw COVID-19 news articles, processed these articles by removing stop words, generated a word occurrence matrix by counting the frequency of each word in each article, then generated a word co-occurrence matrix by calculating the Gram matrix of the occurrence matrix.

The pipeline used a combination of Hadoop MapReduce, Spark, OpenMP, and MPI.

Read more about our algorithm and results at our website, [https://shravnages.github.io/cs205_fp/](https://shravnages.github.io/cs205_fp/).

### Usage

First, clone this repository:

	git clone https://github.com/shravnages/cs205_fp

### MapReduce instructions:

1.  workflow to preprocess text data  
    **Input**: raw news articles stored in HDFS  
    **Output**: NLP preprocessed and cleaned news articles  
    **Code file name**:  
    `mapper.py`  
    `reducer.py`  
    **Execution command**:  
    Mapreduce Streaming on AWS EMR
2.  workflow to count words:  
    **Input**: NLP preprocessed and cleaned news articles  
    **Output**: (word, count) pairs  
    **Code file name**:  
    `mapper1.py`  
    `reducer1.py`  
    **Execution command**:  
    Mapreduce Streaming on AWS EMR
3.  workflow to search for the most frequent N words:  
    **Input**: (word, count) pairs  
    **Output**: most frequent N words  
    **Code file name**:  
    `mapper2.py`  
    `reducer2.py`  
    **Execution command**:  
    Mapreduce Streaming on AWS EMR

### Spark instructions:

Workflow to preprocess data, create top words dictionary, and generate word count matrix  
**Input**: raw news articles stored in HDFS  
**Output**: 1. A dictionary containing the most frequent word in the NLP preprocessed and cleaned news articles. 2. Word count matrix based on the clean news articles and the dictionary  
**Code file name**:  
`sparkaws.py`  
**Execution command**:  
`$ spark-submit --num-executors X --executor-cores Y sparkaws.py`  
(assuming a csv file named data.csv containing raw news article are already put onto HDFS)

### MPI Instructions

**MPI XTX (Gram) matrix**:  
`$ mpicc gram_mpi.c -o gram_mpi -O3`  
`$ mpirun -np [num processes] ./gram_mpi [input csv file] [rows of input] [cols of input]`  
  
Example:  
`$ mpirun -np 8 ./gram_mpi m.csv 41771 20`  
  
**MPI Total Pipeline**:  
`$ mpic++ mpi_pipeline.cpp -o mpi_pipeline -O3`  
OR  
`$ mpic++ mpi_pipeline_hashmap.cpp -o mpi_pipeline -O3`  
  
`$ mpirun -np [num processes] ./mpi_pipeline [input corpus] [dictionary size]`  
  
Example:  
`$ mpirun -np 8 ./mpi_pipeline corpus_big.txt 20`

### OpenMP Instructions

**OpenMP Word Occurrence Matrix**:  
`$ gcc -fopenmp -O3 create_X_omp.c -o create_X`  
`$ ./create_X corpus_big.txt > x_mat.txt`  
  
**OpenMP XTX (Gram) matrix**  
`$ gcc -fopenmp -O3 XT_X_omp.c -o XT_X`  
`$ ./XT_X x_mat.txt`
