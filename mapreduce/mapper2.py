#!/usr/bin/python
import sys

for line in sys.stdin:
    wd,ct = line.split('\t')
    print('{}\t{}'.format(ct, wd))

