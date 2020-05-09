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

stopwords = ['a', 'about', 'above', 'after', 'again', 'against', 'ain', 'all',
        'am', 'an', 'and', 'any', 'are', 'aren', "aren't", 'as', 'at',
        'be', 'because', 'been', 'before', 'being', 'below', 'between',
        'both', 'but', 'by', 'can', 'couldn', "couldn't", 'd', 'did',
        'didn', "didn't", 'do', 'does', 'doesn', "doesn't", 'doing',
        'don', "don't", 'down', 'during', 'each', 'few', 'for', 'from',
        'further', 'had', 'hadn', "hadn't", 'has', 'hasn', "hasn't",
        'have', 'haven', "haven't", 'having', 'he', 'her', 'here',
        'hers', 'herself', 'him', 'himself', 'his', 'how', 'i', 'if',
        'in', 'into', 'is', 'isn', "isn't", 'it', "it's", 'its',
        'itself', 'just', 'll', 'm', 'ma', 'me', 'mightn', "mightn't",
        'more', 'most', 'mustn', "mustn't", 'my', 'myself', 'needn',
        "needn't", 'no', 'nor', 'not', 'now', 'o', 'of', 'off', 'on',
        'once', 'only', 'or', 'other', 'our', 'ours', 'ourselves', 'out',
        'over', 'own', 're', 's', 'same', 'shan', "shan't", 'she',
        "she's", 'should', "should've", 'shouldn', "shouldn't", 'so',
        'some', 'such', 't', 'than', 'that', "that'll", 'the', 'their',
        'theirs', 'them', 'themselves', 'then', 'there', 'these', 'they',
        'this', 'those', 'through', 'to', 'too', 'under', 'until', 'up',
        've', 'very', 'was', 'wasn', "wasn't", 'we', 'were', 'weren',
        "weren't", 'what', 'when', 'where', 'which', 'while', 'who',
        'whom', 'why', 'will', 'with', 'won', "won't", 'wouldn',
        "wouldn't", 'y', 'you', "you'd", "you'll", "you're", "you've",
        'your', 'yours', 'yourself', 'yourselves', 'could', "he'd",
        "he'll", "he's", "here's", "how's", "i'd", "i'll", "i'm", "i've",
        "let's", 'ought', "she'd", "she'll", "that's", "there's",
        "they'd", "they'll", "they're", "they've", "we'd", "we'll",
        "we're", "we've", "what's", "when's", "where's", "who's",
        "why's", 'would', 'able', 'abst', 'accordance', 'according',
        'accordingly', 'across', 'act', 'actually', 'added', 'adj',
        'affected', 'affecting', 'affects', 'afterwards', 'ah', 'almost',
        'alone', 'along', 'already', 'also', 'although', 'always',
        'among', 'amongst', 'announce', 'another', 'anybody', 'anyhow',
        'anymore', 'anyone', 'anything', 'anyway', 'anyways', 'anywhere',
        'apparently', 'approximately', 'arent', 'arise', 'around',
        'aside', 'ask', 'asking', 'auth', 'available', 'away', 'awfully',
        'b', 'back', 'became', 'become', 'becomes', 'becoming',
        'beforehand', 'begin', 'beginning', 'beginnings', 'begins',
        'behind', 'believe', 'beside', 'besides', 'beyond', 'biol',
        'brief', 'briefly', 'c', 'ca', 'came', 'cannot', "can't",
        'cause', 'causes', 'certain', 'certainly', 'co', 'com', 'come',
        'comes', 'contain', 'containing', 'contains', 'couldnt', 'date',
        'different', 'done', 'downwards', 'due', 'e', 'ed', 'edu',
        'effect', 'eg', 'eight', 'eighty', 'either', 'else', 'elsewhere',
        'end', 'ending', 'enough', 'especially', 'et', 'etc', 'even',
        'ever', 'every', 'everybody', 'everyone', 'everything',
        'everywhere', 'ex', 'except', 'f', 'far', 'ff', 'fifth', 'first',
        'five', 'fix', 'followed', 'following', 'follows', 'former',
        'formerly', 'forth', 'found', 'four', 'furthermore', 'g', 'gave',
        'get', 'gets', 'getting', 'give', 'given', 'gives', 'giving',
        'go', 'goes', 'gone', 'got', 'gotten', 'h', 'happens', 'hardly',
        'hed', 'hence', 'hereafter', 'hereby', 'herein', 'heres',
        'hereupon', 'hes', 'hi', 'hid', 'hither', 'home', 'howbeit',
        'however', 'hundred', 'id', 'ie', 'im', 'immediate',
        'immediately', 'importance', 'important', 'inc', 'indeed',
        'index', 'information', 'instead', 'invention', 'inward', 'itd',
        "it'll", 'j', 'k', 'keep', 'keeps', 'kept', 'kg', 'km', 'know',
        'known', 'knows', 'l', 'largely', 'last', 'lately', 'later',
        'latter', 'latterly', 'least', 'less', 'lest', 'let', 'lets',
        'like', 'liked', 'likely', 'line', 'little', "'ll", 'look',
        'looking', 'looks', 'ltd', 'made', 'mainly', 'make', 'makes',
        'many', 'may', 'maybe', 'mean', 'means', 'meantime', 'meanwhile',
        'merely', 'mg', 'might', 'million', 'miss', 'ml', 'moreover',
        'mostly', 'mr', 'mrs', 'much', 'mug', 'must', 'n', 'na', 'name',
        'namely', 'nay', 'nd', 'near', 'nearly', 'necessarily',
        'necessary', 'need', 'needs', 'neither', 'never', 'nevertheless',
        'new', 'next', 'nine', 'ninety', 'nobody', 'non', 'none',
        'nonetheless', 'noone', 'normally', 'nos', 'noted', 'nothing',
        'nowhere', 'obtain', 'obtained', 'obviously', 'often', 'oh',
        'ok', 'okay', 'old', 'omitted', 'one', 'ones', 'onto', 'ord',
        'others', 'otherwise', 'outside', 'overall', 'owing', 'p',
        'page', 'pages', 'part', 'particular', 'particularly', 'past',
        'per', 'perhaps', 'placed', 'please', 'plus', 'poorly',
        'possible', 'possibly', 'potentially', 'pp', 'predominantly',
        'present', 'previously', 'primarily', 'probably', 'promptly',
        'proud', 'provides', 'put', 'q', 'que', 'quickly', 'quite', 'qv',
        'r', 'ran', 'rather', 'rd', 'readily', 'really', 'recent',
        'recently', 'ref', 'refs', 'regarding', 'regardless', 'regards',
        'related', 'relatively', 'research', 'respectively', 'resulted',
        'resulting', 'results', 'right', 'run', 'said', 'saw', 'say',
        'saying', 'says', 'sec', 'section', 'see', 'seeing', 'seem',
        'seemed', 'seeming', 'seems', 'seen', 'self', 'selves', 'sent',
        'seven', 'several', 'shall', 'shed', 'shes', 'show', 'showed',
        'shown', 'showns', 'shows', 'significant', 'significantly',
        'similar', 'similarly', 'since', 'six', 'slightly', 'somebody',
        'somehow', 'someone', 'somethan', 'something', 'sometime',
        'sometimes', 'somewhat', 'somewhere', 'soon', 'sorry',
        'specifically', 'specified', 'specify', 'specifying', 'still',
        'stop', 'strongly', 'sub', 'substantially', 'successfully',
        'sufficiently', 'suggest', 'sup', 'sure', 'take', 'taken',
        'taking', 'tell', 'tends', 'th', 'thank', 'thanks', 'thanx',
        'thats', "that've", 'thence', 'thereafter', 'thereby', 'thered',
        'therefore', 'therein', "there'll", 'thereof', 'therere',
        'theres', 'thereto', 'thereupon', "there've", 'theyd', 'theyre',
        'think', 'thou', 'though', 'thoughh', 'thousand', 'throug',
        'throughout', 'thru', 'thus', 'til', 'tip', 'together', 'took',
        'toward', 'towards', 'tried', 'tries', 'truly', 'try', 'trying',
        'ts', 'twice', 'two', 'u', 'un', 'unfortunately', 'unless',
        'unlike', 'unlikely', 'unto', 'upon', 'ups', 'us', 'use', 'used',
        'useful', 'usefully', 'usefulness', 'uses', 'using', 'usually',
        'v', 'value', 'various', "'ve", 'via', 'viz', 'vol', 'vols',
        'vs', 'w', 'want', 'wants', 'wasnt', 'way', 'wed', 'welcome',
        'went', 'werent', 'whatever', "what'll", 'whats', 'whence',
        'whenever', 'whereafter', 'whereas', 'whereby', 'wherein',
        'wheres', 'whereupon', 'wherever', 'whether', 'whim', 'whither',
        'whod', 'whoever', 'whole', "who'll", 'whomever', 'whos',
        'whose', 'widely', 'willing', 'wish', 'within', 'without',
        'wont', 'words', 'world', 'wouldnt', 'www', 'x', 'yes', 'yet',
        'youd', 'youre', 'z', 'zero', "a's", "ain't", 'allow', 'allows',
        'apart', 'appear', 'appreciate', 'appropriate', 'associated',
        'best', 'better', "c'mon", "c's", 'cant', 'changes', 'clearly',
        'concerning', 'consequently', 'consider', 'considering',
        'corresponding', 'course', 'currently', 'definitely',
        'described', 'despite', 'entirely', 'exactly', 'example',
        'going', 'greetings', 'hello', 'help', 'hopefully', 'ignored',
        'inasmuch', 'indicate', 'indicated', 'indicates', 'inner',
        'insofar', "it'd", 'keep', 'keeps', 'novel', 'presumably',
        'reasonably', 'second', 'secondly', 'sensible', 'serious',
        'seriously', 'sure', "t's", 'third', 'thorough', 'thoroughly',
        'three', 'well', 'wonder', 'including', 'prime']

dic_size = 20

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


# remove urls
df = df.filter(~df._c3.like('http%'))


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
remover = StopWordsRemover(inputCol="words", outputCol="words1", stopWords=stopwords)
df2 = remover.transform(df1)
#df2.select("words1").show()


# count vectorize
cv = CountVectorizer(inputCol="words1", outputCol="words2", vocabSize=dic_size)
model = cv.fit(df2)
df3 = model.transform(df2)
#df3.select("words2").show(truncate=False)

# save the word dictionary
np.savetxt('word_dic.txt', model.vocabulary, fmt='%s')


# from sparse vector to dense vector
df4 = df3.select("words2")
df5 = df4.select(vector_udf('words2').alias('words3'))
#df5.show(truncate=False)


# split into columns
df6 = df5.select(*(col('words3').getItem(i).alias(str(i)) for i in range(dic_size)))
#df6.coalesce(1).write.csv("outputremote1/")
df6.write.format("csv").save("outputaws14/")


# potential way of doing matrix multiplcation, but not so efficient compared to MPI

#df7.show(truncate=False)

# # matrix multiplication
# colDFs = []
# for c2 in df6.columns:
# 	colDFs.append( df6.select( [ F.sum(df6[c1]*df6[c2]).alias('op_{0}'.format(i)) for i,c1 in enumerate(df6.columns) ] ) )

# mtxDF = functools.reduce(lambda a,b: a.select(a.columns).union(b.select(a.columns)), colDFs )
# # mtxDF.show()

# # mtxDF.write.format("csv").save('output2/')
# mtxDF.coalesce(1).write.csv("outputfinal2/")






