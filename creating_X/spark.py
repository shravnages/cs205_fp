from pyspark import SparkConf, SparkContext
import string

conf = SparkConf().setMaster('local').setAppName('Grep')
sc = SparkContext(conf = conf)

RDDvar = sc.textFile("corpus.txt")

dictionary = RDDvar.flatMap(lambda line: line.lower().split()).distinct().collect()

words_in_sentences = RDDvar.map(lambda line: line.split())

X = words_in_sentences.map(lambda line: [line.count(word) for word in dictionary])

# movie_avg_aggreg = movie_avgs.reduceByKey(lambda a, b: a+", "+b)

X.saveAsTextFile("X_spark.txt")
