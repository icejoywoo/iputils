#!/bin/env python
# encoding: utf-8

import _iputils

ip = _iputils.IP2Location("../dict/czip.txt")
print ip.get_country("180.214.232.50")


def parse(line):
    fields = line.split()
    return tuple(fields[:3])

ip = _iputils.IP2Location("../dict/cz88ip.txt", parse)
print ip.get_country("180.214.232.50")

