#!/usr/bin/python

import sys

prev = None
total = 0

for line in sys.stdin:
    key, value = line.split('\t')
    
    if key != prev:
        if prev is not None:
        	print('{}\t{}'.format(prev, total))
        prev = key
        total = 0
    
    total = total + int( value )

print('{}\t{}'.format(prev, total))
