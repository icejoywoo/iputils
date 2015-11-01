#!/bin/env python
# encoding: utf-8
import _iputils

# 加载字典
_iputils.load_dict("../dict/czip.txt")

# 通过ip查找国家
print _iputils.get_country("180.214.232.50")

# 清空内存中的字典数据，释放内存
_iputils.cleanup()

_iputils.load_dict(1)

