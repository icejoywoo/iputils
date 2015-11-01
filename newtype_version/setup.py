#!/usr/bin/env python
#-*- coding:utf-8 -*-

from setuptools import setup, Extension

iputils = Extension('_iputils', sources=["iputilsmodule.cpp"])
setup(ext_modules=[iputils])
