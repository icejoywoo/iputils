#!/bin/env python
# encoding: utf-8
import _iputils

# 加载字典
_iputils.load_dict("../dict/czip.txt")

# 通过ip查找国家
print _iputils.get_country("180.214.232.50")

try:
    _iputils.get_country(1)
except TypeError:
    print "ok"

# 清空内存中的字典数据，释放内存
_iputils.cleanup()

try:
    _iputils.load_dict(1)
except TypeError:
    print "ok"


try:
    _iputils.load_dict("dict/czip.txt")
except ValueError:
    print "file not exist: ok"
