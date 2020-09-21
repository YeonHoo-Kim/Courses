#!/usr/bin/env python

# import modules
from itertools import groupby
from operator import itemgetter
import sys

# 'file' in this case is STDIN
def read_mapper_output(file, separator='\t'):
    # Go through each line
    for line in file:
        # Strip out the separator character
        yield line.rstrip().split(separator)

def main(separator='\t'):
    # Read the data using read_mapper_output
    data = read_mapper_output(sys.stdin, separator=separator)
    # Group words and counts into 'group'
    #   Since MapReduce is a distributed process, each word
    #   may have multiple counts. 'group' will have all counts
    #   which can be retrieved using the word as the key.
    maxNum = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
    for group, current_num in data:
        try:
            if float(maxNum[int(group)]) < float(current_num):
                maxNum[int(group)] = float(current_num)
        except ValueError:
            # Count was not a number, so do nothing
            pass
    for i in range(0,16):
        print("%s%s%f" % (i, separator, maxNum[i]))
if __name__ == "__main__":
    main()