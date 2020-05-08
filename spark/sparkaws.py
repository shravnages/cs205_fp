from pyspark.sql import SparkSession
from pyspark.sql.functions import regexp_replace, lower
from pyspark.sql.functions import udf, split, col
from pyspark.ml.feature import StopWordsRemover
from pyspark.ml.feature import Tokenizer, RegexTokenizer
from pyspark.ml.feature import CountVectorizer
from pyspark.sql import functions as F
from pyspark.ml.linalg import SparseVector, DenseVector
from pyspark.sql.types import ArrayType, DoubleType
import functools
import numpy as np

dic_size=20

spark = SparkSession \
 .builder \
 .appName("word_process") \
 .config("spark.some.config.option", "some-value")\
 .getOrCreate()

#  .config("spark.dynamicAllocation.enabled", "false") \

vector_udf = udf(lambda vector: vector.toArray().tolist(),ArrayType(DoubleType()))


df = spark.read.format("csv").option("header", "false").load('data.csv')

# drop na rows
df = df.na.drop()

# remove duplicates
df = df.drop_duplicates(subset=['_c3'])

# select only news column
df = df.select("_c3")

# remove commas
df = df.withColumn("_c3s", regexp_replace(col('_c3'), '[^\sa-zA-Z0-9]', ""))

# lower case
df = df.withColumn("_c3s", lower(col("_c3s")))
# df.select("_c3s").show(20, False)


# tokenize
tk = RegexTokenizer(inputCol="_c3s", outputCol="words",pattern="\\W+")
df1 = tk.transform(df)
#df1.select("words").show(20, False)


# remove stop words
remover = StopWordsRemover(inputCol="words", outputCol="words1")
df2 = remover.transform(df1)
#df2.select("words1").show()

# count vectorize
cv = CountVectorizer(inputCol="words1", outputCol="words2", vocabSize=dic_size)
model = cv.fit(df2)
df3 = model.transform(df2)
#df3.select("words2").show(truncate=False)

# save the dictionary
np.savetxt('word_dic.txt', model.vocabulary, fmt='%s')

# from sparse vector to dense vector
df4 = df3.select("words2")
df5 = df4.select(vector_udf('words2').alias('words3'))
#df5.show(truncate=False)


# split into columns
df6 = df5.select(*(col('words3').getItem(i).alias(str(i)) for i in range(dic_size)))
#df6.coalesce(1).write.csv("outputremote1/")
df6.write.format("csv").save("outputremoate12/")

#df7.show(truncate=False)

# # matrix multiplication
# colDFs = []
# for c2 in df6.columns:
# 	colDFs.append( df6.select( [ F.sum(df6[c1]*df6[c2]).alias('op_{0}'.format(i)) for i,c1 in enumerate(df6.columns) ] ) )

# mtxDF = functools.reduce(lambda a,b: a.select(a.columns).union(b.select(a.columns)), colDFs )
# # mtxDF.show()

# # mtxDF.write.format("csv").save('output2/')
# mtxDF.coalesce(1).write.csv("outputfinal2/")






