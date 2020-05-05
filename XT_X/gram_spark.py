# https://stackoverflow.com/questions/44348527/matrix-multiplication-at-a-in-pyspark

import sys
from pyspark.sql import SparkSession
from pyspark.sql import functions as F

# Setup spark variables
#conf = SparkConf().setMaster('local[4]').setAppName('P21')
#sc = SparkContext(conf = conf)

spark = SparkSession.builder \
	.master('local[4]') \
	.appName('Gram Matrix X^T * X') \
	.getOrCreate()

inputfile = sys.argv[1]

# Read matrix from text file
df = spark.read.csv(inputfile, sep = ' ', inferSchema = True)

colDFs = []
for c2 in df.columns:
	colDFs.append( df.select( [ F.sum(df[c1]*df[c2]).alias('op_{0}'.format(i)) for i,c1 in enumerate(df.columns) ] ) )

mtxDF = reduce(lambda a,b: a.select(a.columns).union(b.select(a.columns)), colDFs )
mtxDF.show()
