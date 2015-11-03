#!/bin/env python
# encoding: utf-8

import sys
import timeit

sys.path.append('./py_version')
sys.path.append('./newtype_version')

def timeit_wrapper(name, stmt, setup, number=1000000):
    print "%s: %.4f/%d s" % (name, timeit.timeit(stmt, setup, number=number), number)

print "load dict perf:"

try:
    # pypy not support c extension
    import _iputils
    timeit_wrapper('c ext load dict', '_iputils.IP2Location("dict/czip.txt")', 'import _iputils', number=10)
    timeit_wrapper('c ext load dict with user-defined parse', '_iputils.IP2Location("dict/cz88ip.txt", parse)', '''
    import _iputils

    def parse(line):
        fields = line.split()
        return tuple(fields[:3])
    ''', number=10)
except ImportError:
    pass

timeit_wrapper('py load dict', 'iputils.IP2Location("dict/czip.txt")', 'import iputils', number=10)
timeit_wrapper('py load dict with user-defined parse', 'iputils.IP2Location("dict/cz88ip.txt", parse)', '''
import iputils

def parse(line):
    fields = line.split()
    return tuple(fields[:3])
''', number=10)

print "query perf:"

try:
    import _iputils
    timeit_wrapper('c ext query', 'ip.get_country("180.214.232.50")', 'import _iputils; ip = _iputils.IP2Location("dict/czip.txt")')
    timeit_wrapper('c ext query(load dict with user-defined parse)', 'ip.get_country("180.214.232.50")', '''
    import _iputils

    def parse(line):
        fields = line.split()
        return tuple(fields[:3])

    ip = _iputils.IP2Location("dict/cz88ip.txt", parse)
    ''')
except ImportError:
    pass

timeit_wrapper('py query', 'ip.get_country("180.214.232.50")', 'import iputils; ip = iputils.IP2Location("dict/czip.txt")')
timeit_wrapper('py query(load dict with user-defined parse)', 'ip.get_country("180.214.232.50")', '''
import iputils

def parse(line):
    fields = line.split()
    return tuple(fields[:3])

ip = iputils.IP2Location("dict/cz88ip.txt", parse)
''')

