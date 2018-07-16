#!/bin/sh
rm -rf build *.o *.so ots_wrap.cpp ots.py ots.pyc

export MACOSX_DEPLOYMENT_TARGET=10.10
python2.7 setup.py build_ext --inplace
