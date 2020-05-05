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
    text = [w.lower() for w in text if w not in stop_words]
    text = [lm.lemmatize(w) for w in text]
    text = ' '.join(text)
    return text


stop_words = set(stopwords.words('english')) 
tk = RegexpTokenizer(r'\w+')
lm = nltk.WordNetLemmatizer()


for i,line in enumerate(sys.stdin):
    line = line.strip().split(',')
    text = line[3]
    text = process_text(text, tk, lm)
    if text != '':
        print('{}\t{}'.format(text, 1)) 