#!/usr/bin/python
import sys
import re

for line in sys.stdin:
	try:
		line = re.sub( r'^\W+|\W+$', '', line )
		wd,ct = line.strip().split('\t')
		print('{}\t{}'.format(ct, wd))
	except:
		continue


