# setup.py

from distutils.core import setup, Extension
import os
os.environ["CC"] = "clang++"
os.environ["CXX"] = "clang++"

setup(name="ots",
      py_modules=['ots'],
      ext_modules=[Extension("_ots",
                     ["ots.i",
                      "../src/Common.cpp",
                      "../src/Attestation.cpp",
                      "../src/DetachedFile.cpp",
                      "../src/Op.cpp",
                      "../src/Timestamp.cpp"],
                     swig_opts=['-c++'],
                     libraries=['crypto']
                  )]
      
)
