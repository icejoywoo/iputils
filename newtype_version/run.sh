#!/bin/bash

set -e

python setup.py build

cp build/lib.*/_iputils.so .

python test.py
