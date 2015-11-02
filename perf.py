#!/bin/env python
# encoding: utf-8

import sys
import timeit

sys.path.append('./py_version')
sys.path.append('./newtype_version')

print timeit.timeit('ip.get_country("180.214.232.50")', 'import _iputils; ip = _iputils.IP2Location("dict/czip.txt")')
print timeit.timeit('ip.get_country("180.214.232.50")', '''
import _iputils

def parse(line):
    fields = line.split()
    return tuple(fields[:3])

ip = _iputils.IP2Location("dict/cz88ip.txt", parse)
''')

print timeit.timeit('ip.get_country("180.214.232.50")', 'import iputils; ip = iputils.IP2Location("dict/czip.txt")')
