#!/bin/env python
# encoding: utf-8

import _iputils

try:
    ip = _iputils.IP2Location("dict/czip.txt")
except ValueError:
    print "file not exist: ok"

ip = _iputils.IP2Location("../dict/czip.txt")
print ip.get_country("180.214.232.50")


def parse(line):
    fields = line.split()
    return tuple(fields[:3])

ip = _iputils.IP2Location("../dict/cz88ip.txt", parse)
print ip.get_country("180.214.232.50")

