# Generate a random binary matrix of a given size
# Run: python3 generate_matrix.py [size]

import sys
import random

size = int(sys.argv[1])

f = open('matrix.txt', 'w')

for i1 in range(size):
	for i2 in range(size):
		n = random.randint(0, 1)
		f.write(str(n) + ' ')
	f.write('\n')
f.close()
