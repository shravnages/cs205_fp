#!/usr/bin/python

import sys
import heapq

dic_size = 2
lis = []
for line in sys.stdin:
    ct, wd = line.split('\t')
    ct = int(ct)
    
    if len(lis) < dic_size:
        heapq.heappush(lis, (ct, wd))
        
    else:
        min_ct = lis[0][0]
        
        if min_ct < ct:
            heapq.heapreplace(lis, (ct, wd))

lis = [w[1].strip() for w in lis]
print(lis)



