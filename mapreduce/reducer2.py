#!/usr/bin/python

import sys

dic_size=1000

for i,line in enumerate(sys.stdin):

	try:
		ct,wd = line.split('\t')
		print(wd)
		if i >= dic_size: 
			break
	except:
		pass
