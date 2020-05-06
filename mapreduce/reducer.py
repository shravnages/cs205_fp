#!/usr/bin/python

import sys

prev = None

for line in sys.stdin:
    key, value = line.split('\t')
    
    if key != prev:
        if prev is not None:
        	print(prev)
        prev = key

print(prev)

