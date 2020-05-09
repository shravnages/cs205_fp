#!/usr/bin/python

import sys
import re

for line in sys.stdin:
	try:
	    line = re.sub( r'^\W+|\W+$', '', line )
	    for word in line.split():
	        print('{}\t{}'.format(word, 1))
	except:
		continue