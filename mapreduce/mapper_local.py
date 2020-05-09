#!/usr/bin/python

from nltk.corpus import stopwords 
from nltk.tokenize import RegexpTokenizer
import nltk
import re
import string
import pandas as pd
import sys


def process_text(text, tk, lm):
    text = tk.tokenize(text)
    text = [w.lower() for w in text if w.lower() not in stop_words]
    text = [w.lower() for w in text if w.lower() not in meaningless_words]
    text = [lm.lemmatize(w) for w in text]
    text = ' '.join(text)
    return text


meaningless_words = set(['said', 'like', 'take', 'get', 'make', 'across', 'show',
    'also', 'back', 'due'])

stop_words = set(stopwords.words('english')) 
tk = RegexpTokenizer(r'\w+')
lm = nltk.WordNetLemmatizer()


# if 'the' in stop_words:
#     print('hahah')

for i,line in enumerate(sys.stdin):

    try:
        line = re.sub( r'^\W+|\W+$', '', line)
        line = line.strip().split(',')
        text = line[3]
        text1 = process_text(text, tk, lm)
        if text1 != '':
            print('{}\t{}'.format(text1, 1)) 
    except:
        pass